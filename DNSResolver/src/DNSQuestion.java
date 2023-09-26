import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Objects;

/**
 * Question section format
 *                                     1  1  1  1  1  1
 *       0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                                               |
 *     /                     QNAME                     /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                     QTYPE                     |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                     QCLASS                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 * This class represents a client request
 */

public class DNSQuestion {
    protected byte[] QName;     // a domain name represented as a sequence of labels
    private short QType;        // a two octet code which specifies the type of the query
    private short QClass;       // a two octet code that specifies the class of the query

    /**
     * read a question from the input stream
     * Due to compression, you may have to ask the DNSMessage containing this question to read some of the fields
     * @param inputStream
     * @param dnsMessage
     * @return DNSQuestion
     */
    public static DNSQuestion decodeQuestion(InputStream inputStream, DNSMessage dnsMessage) throws IOException {
        DNSQuestion dnsQuestion = new DNSQuestion();
        ByteArrayOutputStream byteArrayOutputStream = dnsMessage.readDomainName(inputStream);

        dnsQuestion.QName = byteArrayOutputStream.toByteArray();
        dnsQuestion.QType = new BigInteger(inputStream.readNBytes(2)).shortValue();
        dnsQuestion.QClass = new BigInteger(inputStream.readNBytes(2)).shortValue();

        return dnsQuestion;
    }

    /**
     * Write the question bytes which will be sent to the client
     * The hash map is used for us to compress the message, see the DNSMessage class below
     * @param byteArrayOutputStream
     * @param domainNameLocations
     */
    public void writeBytes(ByteArrayOutputStream byteArrayOutputStream, HashMap<String,Integer> domainNameLocations) throws IOException {
        int location = byteArrayOutputStream.size();

        for (int i = 0; i < QName.length; i++) {
            byteArrayOutputStream.write(QName[i]);
        }
        byteArrayOutputStream.write(DNSMessage.shortToByteArr(QType));
        byteArrayOutputStream.write(DNSMessage.shortToByteArr(QClass));

        String hashStr = new String(QName);
        domainNameLocations.put(hashStr, location);
    }

    // Let your IDE generate these
    // They're needed to use a question as a HashMap key, and to get a human readable string
    @Override
    public String toString() {
        String result = "DNSQuestion{" +
                        " QName=" + Arrays.toString(QName) +
                        ", QType=" + QType +
                        ", QClass=" + QClass +
                        '}';
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;
        DNSQuestion that = (DNSQuestion) obj;
        return QType == that.QType &&
                QClass == that.QClass &&
                Arrays.equals(QName, that.QName);
    }

    @Override
    public int hashCode() {
        int result = Objects.hash(QType, QClass);
        result = 31 * result + Arrays.hashCode(QName);
        return result;
    }
}
