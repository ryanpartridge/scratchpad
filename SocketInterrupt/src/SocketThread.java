import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.UnsupportedCharsetException;
import java.util.Iterator;
import java.util.Set;

public class SocketThread extends Thread
{
    private InetSocketAddress localAddr;
    private Charset iso8859;
    private Selector selector = null;
    private SelectionKey serverKey = null, agentKey = null;
    private ByteBuffer buffer;

    public SocketThread()
    {
        super("SocketThread");
        localAddr = new InetSocketAddress(5111);
        try
        {
            buffer = ByteBuffer.allocate(4096);
            buffer.mark();
            iso8859 = Charset.forName("ISO-8859-1");
        }
        catch (UnsupportedCharsetException uce)
        {
            // this character set should always be supported
        }
    }

    @Override
    public void run()
    {
        System.out.println("socket thread started");
        try
        {
            selector = Selector.open();
            ServerSocketChannel listenChannel = ServerSocketChannel.open();
            listenChannel.configureBlocking(false);
            listenChannel.setOption(StandardSocketOptions.SO_REUSEADDR, true);
            listenChannel.bind(localAddr);
            serverKey = listenChannel.register(selector, SelectionKey.OP_ACCEPT);
            doSelect();
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
            if (agentKey != null)
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
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }

    public void handleRead(SocketChannel channel)
    {
        try
        {
            //ByteBuffer buffer = ByteBuffer.allocate(4096);
            buffer.mark();
            int numRead = channel.read(buffer);
            if (numRead > 0)
            {
                System.out.println("read " + numRead + " bytes");
                boolean cfFound = false;
                //buffer.limit(buffer.position());
                buffer.reset();
                for (int i = 0; i < numRead - 1; ++i)
                {
                    byte b1 = buffer.get();
                    byte b2 = buffer.get(buffer.position());
                    System.out.println("buffer[" + (buffer.position() - 1) + "] = " + (char)b1 + "\tbuffer[" + buffer.position() + "] = " + (char)b2);
                    if (b1 == '\r' && b2 == '\n')
                    {
                        cfFound = true;
                        String message = new String(buffer.array(), 0, buffer.position());
                        System.out.println("incoming message: " + message.trim());
                        ByteBuffer tmpBuffer = ByteBuffer.allocate(4096);
                        tmpBuffer.mark();
                        tmpBuffer.put(buffer);
                        buffer = tmpBuffer;
                    }
                }
                if (!cfFound)
                {
                    buffer.mark();
                }
            }
            else
            {
                System.out.println("broken connection");
                channel.close();
                agentKey.cancel();
                agentKey = null;
            }
        }
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }
}
