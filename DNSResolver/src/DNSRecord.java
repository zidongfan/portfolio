import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.Calendar;
import java.util.HashMap;

/**
 * Resource record format
 *      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                                               |
 *     /                                               /
 *     /                      NAME                     /
 *     |                                               |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                      TYPE                     |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                     CLASS                     |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                      TTL                      |
 *     |                                               |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                   RDLENGTH                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
 *     /                     RDATA                     /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 * Everything after the header and question parts of the DNS message are stored as records
 * This should have all the fields listed in the spec as well as a Date object storing when this record was created by your program
 */

public class DNSRecord {
    private byte leadByte;
    private byte offsetByte;
    private byte[] name;           // a domain name to which this resource record pertains
    private short type;            // two octets containing one of the RR type codes
    private short rClass;           // two octets which specify the class of the data in the RDATA field
    private int ttl;                // a 32 bit unsigned integer that specifies the time interval (in seconds)
    private short dLength;         // an unsigned 16 bit integer that specifies the length in octets of the RDATA field
    private byte[] data;           // a variable length string of octets that describes the resource

    private Calendar expirationDate;



    /**
     *
     * @param inputStream
     * @param dnsMessage
     * @return DNSRecord
     */
    public static DNSRecord decodeRecord(InputStream inputStream, DNSMessage dnsMessage) throws Exception {
        DNSRecord dnsRecord = new DNSRecord();
        dnsRecord.expirationDate = Calendar.getInstance();

        dnsRecord.leadByte = (byte)(inputStream.read());
        dnsRecord.offsetByte = (byte)(inputStream.read());
        inputStream.mark(2);

        if (dnsRecord.leadByte < 0) {
            if (dnsMessage.getQuestionList().size() > 0) {
                dnsRecord.name = dnsMessage.getQuestionList().get(0).QName;
                dnsRecord.type = new BigInteger(inputStream.readNBytes(2)).shortValue();
                dnsRecord.rClass = new BigInteger(inputStream.readNBytes(2)).shortValue();
            }
        }
        else if (dnsRecord.leadByte == 0 && (byte)(inputStream.read()) == 41) {
            inputStream.reset();
            dnsRecord.leadByte = 0;
            byte b = (byte)(inputStream.read());
            dnsRecord.offsetByte = 0;
            dnsRecord.name = new byte[1];
            Arrays.fill(dnsRecord.name, (byte)(0));
            dnsRecord.type = new BigInteger(inputStream.readNBytes(2)).shortValue();
            dnsRecord.rClass = new BigInteger(inputStream.readNBytes(2)).shortValue();
        }
        else if (dnsRecord.leadByte == 0) {;
            dnsRecord.name = dnsMessage.readDomainName(inputStream).toByteArray();
            dnsRecord.type = new BigInteger(inputStream.readNBytes(2)).shortValue();
            dnsRecord.rClass = new BigInteger(inputStream.readNBytes(2)).shortValue();
        } else
            throw new Exception();

        dnsRecord.ttl = new BigInteger(inputStream.readNBytes(4)).intValue();
        dnsRecord.dLength = new BigInteger(inputStream.readNBytes(2)).shortValue();
        if (dnsRecord.dLength <= 0)
            dnsRecord.data = inputStream.readNBytes(0);
        else
            dnsRecord.data = inputStream.readNBytes(dnsRecord.dLength);

        dnsRecord.expirationDate.add(Calendar.SECOND, dnsRecord.ttl);

        return dnsRecord;
    }

    /**
     *
     * @param byteArrayOutputStream
     * @param hashMap
     */
    public void writeBytes(ByteArrayOutputStream byteArrayOutputStream, HashMap<String, Integer> hashMap) throws IOException {
        String qName = new String(name);
        byte pointer = (byte)(0b11000000);
        ByteBuffer ttlBA = ByteBuffer.allocate(4);
        ttlBA.putInt(ttl);

        if (hashMap.containsKey(qName)) {
            int offset = hashMap.get(qName);
            byteArrayOutputStream.write(pointer);
            byteArrayOutputStream.write(offset);
            byteArrayOutputStream.write(DNSMessage.shortToByteArr(type));
            byteArrayOutputStream.write(DNSMessage.shortToByteArr(rClass));
            byteArrayOutputStream.write(ttlBA.array());
            byteArrayOutputStream.write(DNSMessage.shortToByteArr(dLength));
            byteArrayOutputStream.write(data);
        } else {
            byteArrayOutputStream.write(leadByte);
            byteArrayOutputStream.write(name);
            byteArrayOutputStream.write(41);
            byteArrayOutputStream.write(DNSMessage.shortToByteArr(type));
            byteArrayOutputStream.write(DNSMessage.shortToByteArr(rClass));
            byteArrayOutputStream.write(ttlBA.array());
            byteArrayOutputStream.write(DNSMessage.shortToByteArr(dLength));
            byteArrayOutputStream.write(data);
        }
    }

    @Override
    public String toString() {
        String result = "DNSRecord{" +
                        "leadByte=" + leadByte +
                        ", offsetByte=" + offsetByte +
                        ", name=" + Arrays.toString(name) +
                        ", type=" + type +
                        ", class=" + rClass +
                        ", ttl=" + ttl +
                        ", dLength=" + dLength +
                        ", data=" + Arrays.toString(data) +
                        '}';
        return result;
    }

    /**
     * The Date and Calendar classes will be useful for this
     * @return whether the creation date + the time to live is after the current time
     */
    boolean isExpired(){
        Calendar comparisonTime = Calendar.getInstance();

        return expirationDate.after(comparisonTime);
    }
}
