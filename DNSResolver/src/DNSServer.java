import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.ArrayList;

/**
 * This class should open up a UDP socket (DatagramSocket class in Java), and listen for requests
 * When it gets one, it should look at all the questions in the request
 * If there is a valid answer in cache, add that the response, otherwise create another UDP socket to forward the request Google (8.8.8.8) and then await their response
 * Once you've dealt with all the questions, send the response back to the client
 *
 * Note: dig sends an additional record in the "additionalRecord" fields with a type of 41
 * You're supposed to send this record back in the additional record part of your response as well
 *
 * Note, that in a real server, the UDP packets you receive could be client requests or google responses at any time
 * For our basic testing you can assume that the next UDP packet you get after forwarding your request to Google will be the response from Google
 * To be more robust, you can look at the ID in the request, and keep track of your "in-flight" requests to Google, but you don't need to do that for the assignment
 */

public class DNSServer extends Thread {
    private DatagramSocket socket;
    private byte[] buff = new byte[256];
    private int port = 8053;

    public DNSServer() throws SocketException {
        socket = new DatagramSocket(port);
    }

    public void run() {
        boolean isRunning = true;

        while (isRunning) {
            DatagramPacket packet = new DatagramPacket(buff, buff.length);
            try {
                socket.receive(packet);

                InetAddress address = packet.getAddress();
                int port = packet.getPort();

                DatagramPacket datagramPacket = new DatagramPacket(buff, packet.getLength(), InetAddress.getByName("8.8.8.8"), 53);

                DNSCache dnsCache = new DNSCache();
                DNSMessage dnsMessage = DNSMessage.decodeMessage(buff);
                DNSRecord dnsRecord =  dnsCache.query(dnsMessage.getQuestionList().get(0));
                ArrayList<DNSRecord> recArrList = new ArrayList<>();
                recArrList.add(dnsRecord);

                if (dnsRecord != null) {
                    dnsMessage = DNSMessage.buildResponse(dnsMessage, recArrList);
                    packet = new DatagramPacket(dnsMessage.toBytes(), dnsMessage.toBytes().length, address, port);
                    System.out.println("record cached");
                } else {
                    socket.send(datagramPacket);
                    packet = new DatagramPacket(buff, buff.length);

                    socket.receive(packet);
                    dnsMessage = DNSMessage.decodeMessage(buff);
                    if (dnsMessage.getHeader().getRCode() == 0)
                        dnsCache.insert(dnsMessage.getQuestionList().get(0), dnsMessage.getRecordList().get(0));
                    packet = new DatagramPacket(buff, buff.length, address, port);
                }

                socket.send(packet);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        socket.close();
    }

    public static void main(String[] args) throws SocketException {
        DNSServer dnsServer = new DNSServer();
        System.out.println("DNS Server is running on Port " + dnsServer.port + "...");
        dnsServer.run();
    }
}

// dig example.com @127.0.0.1 -p 8053

/**
 * ; <<>> DiG 9.10.6 <<>> example.com @127.0.0.1 -p 8053
 * ;; global options: +cmd
 * ;; Got answer:
 * ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 22088
 * ;; flags: qr rd ra ad; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 1
 * ;; WARNING: Message has 200 extra bytes at end
 *
 * ;; OPT PSEUDOSECTION:
 * ; EDNS: version: 0, flags:; udp: 512
 * ;; QUESTION SECTION:
 * ;example.com.                   IN      A
 *
 * ;; ANSWER SECTION:
 * example.com.            20600   IN      A       93.184.216.34
 *
 * ;; Query time: 75 msec
 * ;; SERVER: 127.0.0.1#8053(127.0.0.1)
 * ;; WHEN: Wed Feb 06 18:07:44 MST 2023
 * ;; MSG SIZE  rcvd: 256
 */

/**
 * DNS Server is running on Port 8053...
 * DNSHeader{id=[86, 72], qr=0, op_code=0, aa=0, tc=0, rd=1, ra=0, z=0, ad=1, cd=0, rCode=0, qdCount=1, anCount=0, nsCount=0, arCount=1}
 * [DNSQuestion{ QName=[7, 101, 120, 97, 109, 112, 108, 101, 3, 99, 111, 109, 0], QType=1, QClass=1}]
 * []
 * [DNSRecord{leadByte=0, offsetByte=0, name=[0], type=4096, class=0, ttl=0, dLength=0, data=[]}]
 * DNSHeader{id=[86, 72], qr=1, op_code=0, aa=0, tc=0, rd=1, ra=1, z=0, ad=1, cd=0, rCode=0, qdCount=1, anCount=1, nsCount=0, arCount=1}
 * [DNSQuestion{ QName=[7, 101, 120, 97, 109, 112, 108, 101, 3, 99, 111, 109, 0], QType=1, QClass=1}]
 * [DNSRecord{leadByte=-64, offsetByte=12, name=[7, 101, 120, 97, 109, 112, 108, 101, 3, 99, 111, 109, 0], type=1, class=1, ttl=20600, dLength=4, data=[93, -72, -40, 34]}]
 * [DNSRecord{leadByte=0, offsetByte=0, name=[0], type=512, class=0, ttl=0, dLength=0, data=[]}]
 */