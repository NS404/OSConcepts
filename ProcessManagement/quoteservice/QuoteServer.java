import java.net.*;
import java.io.*;

public class QuoteServer
{
    public static void main(String[] args) {
        
        String[] quotes = {"One apple a day keeps the doctor away", 
                            "Well done is better than well said. -Benjamin Franklin", 
                            "You must be the change you wish to see in the world. -Mahatma Gandhi",
                            "The only thing we have to fear is fear itself. -Franklin D. Roosevelt"};        


        try {
            ServerSocket sock = new ServerSocket(6017);
            while(true) {
                Socket client = sock.accept();
                PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
                pout.println(quotes[(int)Math.floor(Math.random() * quotes.length)]);
                client.close();
            }
        }catch (IOException ioe) {
            System.err.println(ioe);
        }
    }
}

