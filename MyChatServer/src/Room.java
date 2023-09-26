/*
* Extra feature:
* Keep tract of the messages sent to each room,
* and when a new client joins,
* send all messages for that room to the client.
 */

import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

public class Room {
    private static HashMap<String, Room> rooms_ = new HashMap();
    private String name_;
    private ArrayList<ConnectionHandler> clients_ = new ArrayList();
    private ArrayList<String> messages_ = new ArrayList();

    // get or set the room
    public synchronized static Room getRoom(String roomName) {
        Room room = (Room) rooms_.get(roomName);
        if (room == null) {
            room = new Room(roomName);
            rooms_.put(roomName, room);
        }
        return room;
    }


    public String getName() {
        return this.name_;
    }

    // add a client, when add client in same room, send previously msg to new client
    public synchronized void addClient(ConnectionHandler ch) throws IOException {
        Iterator iterator = this.clients_.iterator();

        while (iterator.hasNext()) {
            ConnectionHandler connectionHandler = (ConnectionHandler) iterator.next();
            String joinMessage = ChatJson.setJoinMessage(connectionHandler.getUserName(), this.name_);
            ch.sendWebSocketMessage(joinMessage);
        }

        iterator = this.messages_.iterator();

        while (iterator.hasNext()) {
            String msg = (String) iterator.next();
            ch.sendWebSocketMessage(msg);
        }

        PrintStream printStream = System.out;
        String userName = ch.getUserName();
        printStream.println(userName + " has joined room " + this.name_);

        this.clients_.add(ch);
        String joinMessage = ChatJson.setJoinMessage(ch.getUserName(), this.name_);
        this.sendMessage(joinMessage, false);
    }

    // remove a client
    public synchronized void removeClient(ConnectionHandler ch) {
        this.clients_.remove(ch);
    }

    // send message to all clients in the room
    public synchronized void sendMessage(String msg, boolean isSendMsg) throws IOException {
        PrintStream printStream = System.out;
        int clientNum = this.clients_.size();
        printStream.println("Server send this message to " + clientNum + " clients: " + msg);

        Iterator iterator = this.clients_.iterator();

        while (iterator.hasNext()) {
            ConnectionHandler ch = (ConnectionHandler) iterator.next();
            ch.sendWebSocketMessage(msg);
        }

        if (isSendMsg) {
            this.messages_.add(msg);
        }
    }

    // Room constructor
    private Room(String roomName) {
        System.out.println("New chat room: " + roomName);
        this.name_ = roomName;
    }
}
