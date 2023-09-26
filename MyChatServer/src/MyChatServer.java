/* Web Server Class include main function
*
* Create ServerSocket and client socket
*
* Create Threads to run the connection handler
*
 */

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class MyChatServer {

    public static void main(String[] args) throws IOException {

        System.out.println("Server starting...\n");

        ServerSocket serverSocket = null;

        try{
            serverSocket = new ServerSocket(8080);
        } catch (IOException e) {
            System.out.println(e.getMessage());
            System.exit(-1);
        }

        Socket client = null;

        while (true) {
            try {
                client = serverSocket.accept();
                System.out.println("Client connected...\n");
                ConnectionHandler ch = new ConnectionHandler(client);
                Thread thread = new Thread(ch);
                thread.start();

            } catch (Exception e) {
                System.out.println(e.getMessage());
                e.printStackTrace();
            }
        }


    }
}
