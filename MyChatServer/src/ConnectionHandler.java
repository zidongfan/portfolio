import java.io.*;
import java.net.Socket;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;

public class ConnectionHandler implements Runnable {
    private Socket client_;
    private String fileName_;
    private HashMap<String, String> headers_ = new HashMap();

    private Room room_ = null;
    private String userName_ = null;

    public ConnectionHandler(Socket client) {
        this.client_ = client;
    }


    /* run function that is executed by Threads
     *
     * 1. read the client request;
     * 2. get headers which include file name, key field and value;
     * 3. if headers includes web socket key, then this is a web socket request;
     * 4. if not 3, treat as normal http request;
     * 5. set the response content;
     * 6. send response to clients.
     *
     */
    @Override
    public void run() {
        BufferedReader bf = null;
        try {
            bf = new BufferedReader(new InputStreamReader(this.client_.getInputStream()));
            this.getHeader(bf);

            // check it that is a web socket request
            if (this.headers_.containsKey("sec-websocket-key")) {
                System.out.println("This is a Web Socket request...\n");
                this.setWebSocketContent();
                return;
            }

            this.sendResponse(this.client_.getOutputStream());
            this.client_.close();
            System.out.println("Client closed...\n");

        } catch (NoSuchAlgorithmException | IOException e) {
            System.out.println(e.getMessage());
            e.printStackTrace();
        }

    }


    /* get header function:
     * get header handle the request from clients;
     * get the file name and set the file name;
     * storing file name and headers in here.
     */
    private void getHeader(BufferedReader bf) throws IOException {
        String line = bf.readLine();

        // get file name in first line
        this.fileName_ = line.split(" ")[1];

        if (this.fileName_.equals("/")) {
            this.fileName_ = "/index.html";
        }
        this.fileName_ = "resources" + this.fileName_;
        System.out.println("File name is: " + fileName_ + "\n");

        // get the header hash map
        boolean textEnd = false;
        while (!textEnd) {
            line = bf.readLine();
            if (line.equals("")) {
                textEnd = true;
            } else {
                String[] lines = line.split(": ");
                this.headers_.put(lines[0].toLowerCase(), lines[1]);
            }
        }

        // print out header map
        System.out.println("Headers:");
        Iterator it = this.headers_.entrySet().iterator();

        while (it.hasNext()) {
            Map.Entry entry = (Map.Entry) it.next();
            PrintStream ps = System.out;
            String headerKey = (String) entry.getKey();
            ps.println(headerKey + ": " + (String) entry.getValue());
        }
        System.out.println("\n");

    }


    /* if the request is web socket request(if there is ws.open in JavaScript file);
     *
     * get the sec-websocket-key, then use:
     * SHA-1 Hash and Base64 encoding;
     * MessageDigest class will do the SHA-1 hashing;
     * Base64 class will encode the output of the MessageDigest into a string;
     *
     * send the response to client
     * send the web socket messages
     *
     */
    private void setWebSocketContent() throws NoSuchAlgorithmException, IOException {
        OutputStream os = this.client_.getOutputStream();
        String receivedKey = (String) this.headers_.get("sec-websocket-key");
        MessageDigest md = MessageDigest.getInstance("SHA-1");
        String responseKey = Base64.getEncoder().encodeToString(md.digest((receivedKey + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11").getBytes()));

        String response = "HTTP/1.1 101 Switching Protocols\r\n" +
                "Upgrade: websocket\r\n" +
                "Connection: Upgrade\r\n" +
                "Sec-WebSocket-Accept: " + responseKey + "\r\n\r\n";
        os.write(response.getBytes());

        System.out.println("Response Headers:\n" +
                response);
        os.flush();

        try {
            while (true) {
                this.readWebSocketMessage();
            }
        } catch (Exception e) {
            System.out.println("User: " + this.userName_ + " closed the web");

            if (this.room_ != null) {
                this.room_.removeClient(this);
                String leaveMessage = ChatJson.setLeaveMessage(this.userName_, this.room_.getName());
                this.room_.sendMessage(leaveMessage, false);
            }
        }

    }

    /*
     * send the request file to client
     * set the response content
     * if file exists send file bytes
     * if file !exists send 404 Not Found
     */
    private void sendResponse(OutputStream os) throws IOException {
        PrintWriter printWriter = new PrintWriter(os, true);
        File file = new File(this.fileName_);
        String result = "200 OK";
        String type = "unknown";
        String content = null;
        FileInputStream fileInputStream = null;
        long size;
        Date date = new Date();
        System.out.println("Handle request for: " + this.fileName_);


        if (!file.exists()) {
            result = "404 Not Found";
            content = "<html> <head> <title>404 Not Found</title> </head> <body> <h1>404 Not Found</h1> </body> </html>";
            size = (long) content.length();
            type = "text/html";
        } else {
            size = file.length();
            fileInputStream = new FileInputStream(file);
            String[] filenameSplit = fileName_.split("\\.");
            if (filenameSplit.length != 0) {
                String extension = filenameSplit[1];
                if (!extension.equals("html") && !extension.equals("css")) {
                    if (!extension.equals("png") && !extension.equals("jpg")) {
                        if (extension.equals("js")) {
                            type = "text/javascript";
                        }
                    } else {
                        type = "image/" + extension;
                    }
                } else {
                    type = "text/" + extension;
                }
            }
        }

        printWriter.println("HTTP/1.1 " + result);
        printWriter.println("Server: Web Socket Server");
        printWriter.println("Date: " + date);
        printWriter.println("Content-type: " + type);
        printWriter.println("Content-length: " + size);
        printWriter.println("");
        if (content == null) {
            fileInputStream.transferTo(os);
        } else {
            printWriter.println(content);
        }

        printWriter.flush();
    }

    public String getUserName() {
        return this.userName_;
    }


    // read the websocket message and send to all clients
    private void readWebSocketMessage() throws Exception {
        try {
            DataInputStream dataInputStream = new DataInputStream(this.client_.getInputStream());
            byte[] bytes = dataInputStream.readNBytes(2);

            // Byte 0: if first number is 1, means the client socket is closed;
            boolean isFIN = (bytes[0] & 0x80) != 0;
            int Opcode = bytes[0] & 0x0F; // Opcode?
            if (Opcode == 8) {
                throw new Exception("Client Socket Closed");
            }

            // Byte 1: if first number is 1 that means masked;
            // if Byte 1 is 126... Unsigned Short
            // if Byte 1 is 127... Long
            boolean isMasked = (bytes[1] & 0x80) != 0; // Masked?
            long payloadLen = (long) (bytes[1] & 0x7F);

            if (payloadLen == 126) {
                payloadLen = (long) dataInputStream.readUnsignedShort();
            } else if (payloadLen == 127) {
                payloadLen = dataInputStream.readLong();
            }

            // Masking-key, if MASK set to 1
            byte[] maskingKey = dataInputStream.readNBytes(4);
            // DECODED
            byte[] payloadData = dataInputStream.readNBytes((int) payloadLen);

            for (int i = 0; (long) i < payloadLen; ++i) {
                payloadData[i] = (byte) (payloadData[i] ^ maskingKey[i % 4]);
            }

            // get type, user, room, messages
            // and send message to all clients
            String payloadSting = new String(payloadData);
            String[] payloadStingSplit = payloadSting.split(" ", 2);
            String type = payloadStingSplit[0];
            if (type.equals("join")) {
                String[] userAndRoom = payloadStingSplit[1].split(" ", 2);
                this.userName_ = userAndRoom[0];
                String roomName = userAndRoom[1];
                Room room = Room.getRoom(roomName);
                this.room_ = room;
                room.addClient(this);
            } else {
                String message;
                String msg = payloadStingSplit[1];
                message = ChatJson.setSendMessage(this.userName_, this.room_.getName(), msg);
                this.room_.sendMessage(message, true);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    //  send message to this client
    public synchronized void sendWebSocketMessage(String payloadData) throws IOException {
        DataOutputStream outputStream = new DataOutputStream(this.client_.getOutputStream());
        outputStream.writeByte(129);

        if (payloadData.length() < 126) {
            outputStream.write(payloadData.length());
        } else if (payloadData.length() < 65535) {
            outputStream.writeByte(126);
            outputStream.writeShort(payloadData.length());
        } else {
            outputStream.writeByte(127);
            outputStream.writeLong((long) payloadData.length());
        }

        outputStream.write(payloadData.getBytes());
        outputStream.flush();
    }
}