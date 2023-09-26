public class ChatJson {
    public ChatJson() {
    }

    public static String setSendMessage(String userName, String roomName, String msg) {
        return new String("{ \"type\": \"message\", \"user\": \"" + userName + "\", \"room\": \"" + roomName + "\", \"message\": \"" + msg + "\" }");
    }

    public static String setJoinMessage(String userName, String roomName) {
        return new String("{ \"type\": \"join\", \"room\": \"" + roomName + "\", \"user\": \"" + userName + "\" }");
    }

    public static String setLeaveMessage(String userName, String roomName) {
        return new String("{ \"type\": \"leave\", \"room\": \"" + roomName + "\", \"user\": \"" + userName + "\" }");
    }
}
