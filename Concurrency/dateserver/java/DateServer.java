import java.net.*;
import java.io.*;
import java.util.concurrent.TimeUnit;

public class DateServer
{
    public static void main(String[] args)
    {
        try{
            ServerSocket sock = new ServerSocket(6013);
            
            while (true){
                Socket client = sock.accept();
                
                PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
        
                Thread.sleep(5000); 
        
                pout.println(new java.util.Date().toString());
    
                client.close();
            }
        } catch (IOException | InterruptedException e) {
            System.err.println(e);
        }
    }
}
