import java.util.concurrent.LinkedBlockingQueue;


public class StateMachine
{
    private SocketThread socketThread = null;
    private LinkedBlockingQueue<String> toAgent = null, fromAgent = null;

    public StateMachine()
    {
        toAgent = new LinkedBlockingQueue<String>();
        fromAgent = new LinkedBlockingQueue<String>();
    }

    public void start()
    {
        try
        {
            System.out.println("starting thread");
            socketThread = new SocketThread(toAgent, fromAgent);
            socketThread.start();
            System.out.println("giving socket thread time");
            Thread.sleep(30000);
            socketThread.interrupt();
            socketThread.join(5000);
        }
        catch (Exception e)
        {
            System.err.println(e.getMessage());
        }
    }

    public static void main(String[] args)
    {
        System.out.println("main thread started");
        StateMachine si = new StateMachine();
        si.start();
        System.out.println("main thread complete");
    }
}