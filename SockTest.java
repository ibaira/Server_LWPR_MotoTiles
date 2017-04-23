import java.net.Socket;
import java.io.BufferedReader;
import java.io.InputStreamReader;
// import java.io.BufferedWriter;


import java.io.*;
import java.net.*;

public class SockTest {
    public static void main(String[] args) {
        try {
            Socket sock = new Socket("localhost", 8888);
            System.out.println("New socket created");  
            // 10.105.112.154", 8888); //"127.0.0.1", 8888);

            // Your Java Code Verbatim:
            BufferedReader br = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            System.out.println("Reader created");  
            
            
            DataOutputStream outToServer = new DataOutputStream(sock.getOutputStream());
            System.out.println("Sender created");  

            int iterations;
            iterations = 1;
            while(iterations < 1000){
                System.out.println("We enter loop");  

                String test = br.readLine();
                System.out.println("READ FROM SERVER: " + test);  

                outToServer.writeBytes("1,2,3,4,5,6,7,8,9,0\n");
                System.out.println("CLIENT SEENDING: " + test);  

            }

            // String sentence;
            // String modifiedSentence;            
            
            sock.close();
        } catch (Exception ex) {}
    }
}