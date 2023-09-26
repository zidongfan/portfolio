import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * This corresponds to an entire DNS Message. It should contain:
 * the DNS Header
 * an array of questions
 * an array of answers
 * an array of "authority records" which we'll ignore
 * an array of "additional records" which we'll almost ignore
 *
 * You should also store the byte array containing the complete message in this class
 * You'll need it to handle the compression technique described above
 */

public class DNSMessage {
    private DNSHeader dnsHeader;                                        // the DNS Header
    private ArrayList<DNSQuestion> questionList = new ArrayList<>();    // an array of questions
    private ArrayList<DNSRecord> answerList = new ArrayList<>();        // an array of answers
//    private ArrayList<DNSRecord> authRecordsList = new ArrayList<>();   // an array of "authority records" which we'll ignore
    private ArrayList<DNSRecord> additionalRecList = new ArrayList<>(); // an array of "additional records" which we'll almost ignore

    /**
     *
     * @param bytes
     * @return DNSMessage
     * @throws Exception
     */
    public static DNSMessage decodeMessage(byte[] bytes) throws Exception {
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(bytes);
        DNSMessage dnsMessage = new DNSMessage();

        dnsMessage.dnsHeader = DNSHeader.decodeHeader(byteArrayInputStream);

        dnsMessage.questionList.add(DNSQuestion.decodeQuestion(byteArrayInputStream, dnsMessage));

        if (dnsMessage.dnsHeader.getAnCount() > 0) {
            for (int i = 0; i < dnsMessage.dnsHeader.getAnCount(); i ++) {
                dnsMessage.answerList.add(DNSRecord.decodeRecord(byteArrayInputStream, dnsMessage));
            }
        }
        dnsMessage.additionalRecList.add(DNSRecord.decodeRecord(byteArrayInputStream, dnsMessage));

        System.out.println(dnsMessage.dnsHeader.toString());
        System.out.println(dnsMessage.questionList.toString());
        System.out.println(dnsMessage.answerList.toString());
        System.out.println(dnsMessage.additionalRecList.toString());

        return dnsMessage;
    }

    public DNSHeader getHeader() {
        return dnsHeader;
    }

    public ArrayList<DNSQuestion> getQuestionList() {
        return questionList;
    }

    public ArrayList<DNSRecord> getRecordList() {
        return answerList;
    }

    public ArrayList<DNSRecord> getAdditionalRecordList() {
        return additionalRecList;
    }

    /**
     * read the pieces of a domain name starting from the current position of the input stream
     * @param inputStream
     * @return ByteArrayOutputStream
     * @throws IOException
     */
    public ByteArrayOutputStream readDomainName(InputStream inputStream) throws IOException {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        byte length = (byte)inputStream.read();
        byteArrayOutputStream.write(length);

        while (length != 0) {
            byteArrayOutputStream.write(inputStream.readNBytes(length));
            length = (byte)inputStream.read();
            byteArrayOutputStream.write(length);
        }

        byteArrayOutputStream.flush();
        byteArrayOutputStream.close();

        return byteArrayOutputStream;
    }

//    /**
//     * same, but used when there's compression and we need to find the domain from earlier in the message.
//     * This method should make a ByteArrayInputStream that starts at the specified byte and call the other
//     * version of this method
//     * @param firstByte
//     * @return
//     */
//    public ByteArrayOutputStream readDomainName(int firstByte) {
//        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
//        ByteArrayInputStream byteArrayInputStream = readDomainName();
//        return byteArrayOutputStream;
//    }

    /**
     * build a response based on the request and the answers you intend to send back
     * @param request
     * @param answers
     * @return DNSMessage
     */
    public static DNSMessage buildResponse(DNSMessage request, ArrayList<DNSRecord> answers) {
        DNSMessage response = new DNSMessage();
        response.questionList = request.getQuestionList();
        response.answerList = answers;
        response.additionalRecList = request.getAdditionalRecordList();
        response.dnsHeader = DNSHeader.buildHeaderForResponse(request, response);
        return response;
    }

    /**
     * get the bytes to put in a packet and send back
     * @return byte[]
     * @throws IOException
     */
    public byte[] toBytes() throws IOException {
        HashMap<String, Integer> hm = new HashMap<>();
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        dnsHeader.writeBytes(baos);
        questionList.get(0).writeBytes(baos, hm);
        if (answerList.size() > 0)
            answerList.get(0).writeBytes(baos, hm);
        if (additionalRecList.size() > 0)
            additionalRecList.get(0).writeBytes(baos,hm);

        return baos.toByteArray();
    }

    public static byte[] shortToByteArr(short s) {
        ByteBuffer buf = ByteBuffer.allocate(2);
        buf.putShort(s);
        return buf.array();
    }

//    /**
//     * If this is the first time we've seen this domain name in the packet, write it using the DNS encoding
//     * (each segment of the domain prefixed with its length, 0 at the end), and add it to the hash map.
//     * Otherwise, write a back pointer to where the domain has been seen previously.
//     * @param domainLocations
//     * @param domainPieces
//     */
//    public static void writeDomainName(ByteArrayOutputStream, HashMap<String,Integer> domainLocations, String[] domainPieces);

//    /**
//     *  join the pieces of a domain name with dots ([ "utah", "edu"] -> "utah.edu" )
//     * @param pieces
//     * @return String
//     */
//    public String joinDomainName(String[] pieces){
//        return null;
//    }


    @Override
    public String toString() {
        String result = "DNSMessage{" +
                        "dnsHeader=" + dnsHeader +
                        ", questionList=" + questionList +
                        ", answerList=" + answerList +
                        ", additionalRecList=" + additionalRecList +
                        '}';
        return result;
    }
}
