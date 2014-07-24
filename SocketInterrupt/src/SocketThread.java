import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.MulticastSocket;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;
import java.util.concurrent.LinkedBlockingQueue;

public class SocketThread extends Thread
{
    private InetSocketAddress localAddr;
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
        }
        catch (IOException ioe)
        {
            System.err.println(ioe);
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
                if (!isInterrupted())
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

    public void handleRead(SocketChannel channel)
    {
        try
        {
            int numRead = channel.read(buffer);
            if (numRead > 0)
            {
                // System.out.println("read " + numRead + " bytes");
                // System.out.println("limit: " + buffer.limit() +
                // "\tcapacity: " + buffer.capacity());
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
                        System.out.println("incoming message: " + message);
                        toAgent.offer(message);
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
            else
            {
                System.out.println("broken connection");
                channel.close();
                synchronized (lock)
                {
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
                        try
                        {
                            if (agentKey != null && agentKey.isValid())
                            {
                                SocketChannel agentChannel = (SocketChannel)agentKey.channel();
                                ByteBuffer messageBuffer = ByteBuffer.wrap(message.getBytes());
                                agentChannel.write(messageBuffer);
                            }
                            else
                            {
                                System.out.println("no agent connection -- sending via multicast");
                                MulticastSocket mSocket = new MulticastSocket();
                            }
                        }
                        catch (IOException ioe)
                        {
                            System.err.println(ioe);
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
    }
}
