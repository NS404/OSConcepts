import java.util.concurrent.*;
import java.lang.Integer;

class Summation implements Callable<Integer>
{
    private int upper;
    public Summation(int upper){
        this.upper = upper;
    }

    // The thread will execute in this method
    public Integer call(){
        int sum = 0;
        for (int i = 1; i <= upper; i++)
            sum += i;

        return new Integer(sum);
    }
}

class RunnableSummation implements Runnable
{
    public void run(){
        System.out.println("I am a thread.");
    }
}

public class Driver
{
    public static void main(String[] args) {
        int upper = Integer.parseInt(args[0]);

        ExecutorService pool = Executors.newSingleThreadExecutor();
        Future<Integer> result = pool.submit(new Summation(upper));
       
        try{
            System.out.println("sum = " + result.get());
        }catch (InterruptedException | ExecutionException ie) {}

//-------------------Thread Pool Example-------------------------------
    int numTasks = Integer.parseInt(args[0].trim());

    //Create the thread pool
    ExecutorService pool1 = Executors.newCachedThreadPool();
    
    //Run each task using a thread in the pool
    for (int i = 0; i < numTasks; i++)
        pool1.execute(new RunnableSummation());

    //shut down the pool once all threads have completed
    pool1.shutdown();    



//--------------------Runnable Interface--------------------------------

        Thread worker = new Thread(new RunnableSummation());
        worker.start();

        try{
            worker.join();
        }
        catch(InterruptedException ie){}  
    }
}
