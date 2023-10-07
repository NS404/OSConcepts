import java.net.*;
import java.io.*;
import java.util.concurrent.*;

public class DateServerMThread
{
    public static void main(String[] args)
    {
        try{
            ServerSocket sock = new ServerSocket(6013);
            
            while (true){
                Socket client = sock.accept();                
                Thread worker = new Thread(new Respond(client));
                worker.start();
                //worker.join();
    
            }
        } catch (IOException e) {
            System.err.println(e);
        }
    }
}

class Respond implements Runnable {

    private Socket client;
    public Respond(Socket sock){
        this.client = sock;
    }


    public void run(){
        try{
            PrintWriter pout = new PrintWriter(client.getOutputStream(), true);

            Thread.sleep(5000);

            pout.println(new java.util.Date().toString());

            client.close();
        
        }catch (IOException | InterruptedException e) {
            System.err.println(e);
        }
    }
}


