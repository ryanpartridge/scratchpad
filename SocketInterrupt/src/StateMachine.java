
public class StateMachine
{
    SocketThread socketThread = null;

    public StateMachine()
    {
    }

    public void start()
    {
        try
        {

            System.out.println("starting thread");
            socketThread = new SocketThread();
            socketThread.start();
//            System.out.println("main thread going to sleep");
//            Thread.sleep(5000);
//            System.out.println("main thread woke up");
//            System.out.println("interrupting listen thread");
//            socketThread.interrupt();
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