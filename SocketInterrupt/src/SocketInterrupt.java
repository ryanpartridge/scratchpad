import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.StandardSocketOptions;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.UnsupportedCharsetException;

public class SocketInterrupt
{
    private InetSocketAddress localAddr;
    private Charset iso8859;
    
    public SocketInterrupt()
    {
        localAddr = new InetSocketAddress(5111);
        try
        {
            iso8859 = Charset.forName("ISO-8859-1");
        }
        catch (UnsupportedCharsetException uce)
        {
            // this character set should always be supported
        }
    }

    public void listen()
    {
        try
        {
            ServerSocketChannel listenChannel = null;
            listenChannel = ServerSocketChannel.open();
            listenChannel.setOption(StandardSocketOptions.SO_REUSEADDR, true);
            listenChannel.socket().bind(localAddr);
            SocketChannel channel = listenChannel.accept();
            Thread requestThread = new Thread(new Runnable()
            {
                public void run()
                {
                    
                }
            });
            System.out.println("got a connection");
        }
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }

    public static void main(String[] args)
    {
        System.out.println("starting up");
        try
        {
            final SocketInterrupt si = new SocketInterrupt();
            Thread listenThread = new Thread(new Runnable()
            {
                public void run()
                {
                    si.listen();
                }
            });
            System.out.println("starting thread");
            listenThread.start();
            System.out.println("main thread going to sleep");
            Thread.sleep(5000);
            System.out.println("main thread woke up");
            System.out.println("interrupting listen thread");
            listenThread.interrupt();
        }
        catch (Exception e)
        {
            System.err.println(e.getMessage());
        }
        System.out.println("shutting down");
    }
}