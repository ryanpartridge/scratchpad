import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetSocketAddress;
import java.net.MulticastSocket;
import java.net.NetworkInterface;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.Set;
import java.util.concurrent.LinkedBlockingQueue;

public class SocketThread extends Thread
{
    private InetSocketAddress localAddr = null, multicastAddr = null;
    private Selector selector = null;
    private SelectionKey serverKey = null, agentKey = null;
    private ByteBuffer buffer;
    private LinkedBlockingQueue<String> toAgent = null, fromAgent = null;
    private Object lock = null;
    private ToAgentThread toAgentThread = null;

    public SocketThread(LinkedBlockingQueue<String> toAgent, LinkedBlockingQueue<String> fromAgent)
    {
        super("SocketThread");
        localAddr = new InetSocketAddress(5111);
        multicastAddr = new InetSocketAddress("239.255.255.251", 5151);
        this.toAgent = toAgent;
        this.fromAgent = fromAgent;
        buffer = ByteBuffer.allocate(4096);
        buffer.mark();
        lock = new Object();
        toAgentThread = new ToAgentThread();
    }

    @Override
    public void run()
    {
        System.out.println("socket thread started");
        try
        {
            toAgentThread.start();
            selector = Selector.open();
            ServerSocketChannel listenChannel = ServerSocketChannel.open();
            listenChannel.configureBlocking(false);
            listenChannel.setOption(StandardSocketOptions.SO_REUSEADDR, true);
            listenChannel.bind(localAddr);
            serverKey = listenChannel.register(selector, SelectionKey.OP_ACCEPT);
            doSelect();
            toAgentThread.interrupt();
            toAgentThread.join(2000);
        }
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
        catch (InterruptedException ie)
        {
            // System.err.println(ie);
        }
        System.out.println("socket thread complete");
    }

    private void doSelect()
    {
        try
        {
            while (true)
            {
                selector.select();
                if (!interrupted())
                {
                    System.out.println("broke out of select");
                    Set<SelectionKey> selected = selector.selectedKeys();
                    Iterator<SelectionKey> it = selected.iterator();
                    while (it.hasNext())
                    {
                        SelectionKey key = (SelectionKey)it.next();
                        it.remove();
                        if (key == serverKey && key.isAcceptable())
                        {
                            handleAccept((ServerSocketChannel)serverKey.channel());
                        }
                        else if (key == agentKey && key.isReadable())
                        {
                            handleRead((SocketChannel)agentKey.channel());
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }

    private void handleAccept(ServerSocketChannel serverChannel)
    {
        try
        {
            SocketChannel channel = serverChannel.accept();
            System.out.println("got a connection from: "
                            + ((InetSocketAddress)channel.getRemoteAddress()).getHostString());
            synchronized (lock)
            {
                if (agentKey != null && agentKey.isValid())
                {
                    SocketChannel agentChannel = (SocketChannel)agentKey.channel();
                    System.out.println("disconnecting existing connection with: "
                                    + ((InetSocketAddress)agentChannel.getRemoteAddress()).getHostString());
                    agentChannel.close();
                    agentKey.cancel();
                }
                channel.configureBlocking(false);
                agentKey = channel.register(selector, SelectionKey.OP_READ);
            }
        }
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }

    private void handleRead(SocketChannel channel)
    {
        try
        {
            int numRead = channel.read(buffer);
            if (numRead > 0)
            {
                packetize();
            }
            else
            {
                synchronized (lock)
                {
                    System.out.println("broken connection");
                    channel.close();
                    agentKey.cancel();
                    agentKey = null;
                }
            }
        }
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }

    private void packetize()
    {
        buffer.limit(buffer.position());
        buffer.reset();
        while (buffer.position() < buffer.limit() - 1)
        {
            byte b1 = buffer.get();
            byte b2 = buffer.get(buffer.position());
            // System.out.println("buffer[" + (buffer.position() - 1) +
            // "] = " + (char)b1 + "\tbuffer["
            // + (buffer.position()) + "] = " + (char)b2);
            if (b1 == '\r' && b2 == '\n')
            {
                // consume the last byte
                buffer.get();
                String message = new String(buffer.array(), 0, buffer.position()).trim();
                if (!message.isEmpty())
                {
                    System.out.println("incoming message: " + message);
                    fromAgent.offer(message);
                    toAgent.offer(message);
                }
                buffer.compact();
                buffer.limit(buffer.position());
                buffer.rewind();
                buffer.mark();
            }
        }
        if (buffer.position() == buffer.limit())
        {
            buffer.clear();
            buffer.mark();
        }
        else
        {
            buffer.mark();
            buffer.get();
            buffer.limit(buffer.capacity());
        }
    }

    private class ToAgentThread extends Thread
    {
        public ToAgentThread()
        {
            super("ToAgentThread");
        }

        public void run()
        {
            System.out.println("starting toAgent thread");
            try
            {
                while (true)
                {
                    String message = toAgent.take();
                    if (!message.endsWith("\r\n"))
                    {
                        message += "\r\n";
                    }
                    System.out.println("sending to agent: " + message);
                    synchronized (lock)
                    {
                        if (agentKey != null && agentKey.isValid())
                        {
                            try
                            {
                                SocketChannel agentChannel = (SocketChannel)agentKey.channel();
                                ByteBuffer messageBuffer = ByteBuffer.wrap(message.getBytes());
                                agentChannel.write(messageBuffer);
                            }
                            catch (IOException ioe)
                            {
                                System.err.println("error sending on agent connection: " + ioe);
                            }
                        }
                        else
                        {
                            System.out.println("no agent connection -- sending via multicast");
                            multicast(message);
                        }
                    }
                }
            }
            catch (InterruptedException ie)
            {
                // System.err.println(ie);
            }
            System.out.println("toAgent thread shutting down");
        }

        private void multicast(String message)
        {
            try
            {
                MulticastSocket mSocket = new MulticastSocket();
                mSocket.setReuseAddress(true);
                mSocket.setTimeToLive(8);
                mSocket.setLoopbackMode(true);

                DatagramPacket packet = new DatagramPacket(message.getBytes(), message.length(), multicastAddr);

                Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
                boolean sentToReal = false;
                while (interfaces.hasMoreElements())
                {
                    NetworkInterface n = interfaces.nextElement();
                    if (n.supportsMulticast())
                    {
                        boolean useInterface = false;
                        if (n.isVirtual() || n.isLoopback())
                        {
                            useInterface = true;
                        }
                        else if (!sentToReal)
                        {
                            sentToReal = true;
                            useInterface = true;
                        }

                        if (useInterface)
                        {
                            mSocket.setNetworkInterface(n);
                            mSocket.send(packet);
                        }
                    }
                }
                mSocket.close();
            }
            catch (IOException ioe)
            {
                System.err.println("error sending via multicast: " + ioe);
            }
        }
    }
}
