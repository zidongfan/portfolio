import java.io.*;
import java.math.BigInteger;
import java.util.Arrays;

/**
 * Header section format
 *                                     1  1  1  1  1  1
 *       0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                      ID                       |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    QDCOUNT                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    ANCOUNT                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    NSCOUNT                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    ARCOUNT                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 * This class should store all the data provided by the 12 byte DNS header
 * See the spec for all the fields needed
 * https://www.ietf.org/rfc/rfc1035.txt
 *
 * https://www.rfc-editor.org/rfc/rfc5395
 */

public class DNSHeader {
    private byte[] id;          // ID:  16-bit query identifier
    private byte[] flags;       // second row in header (QR, Opcode, AA, TC, RD, RA, Z, RCODE)
    private byte qr;            // QR:  whether this message is a query (0), or a response (1)
    private byte opcode;        // Opcode:  4-bit opcode
    private byte aa;            // AA: Authoritative Answer
    private byte tc;            // TC: TrunCation
    private byte rd;            // RD: Recursion Desired
    private byte ra;            // RA: Recursion Available
    private byte z;             // Z: Reserved for future use
    private byte ad;            // ad: Authentic Data
    private byte cd;            // cd: Checking Disabled
    private byte rCode;         // RCODE:  4-bit response code
    private short qdCount;      // QDCOUNT: the number of questions
    private short anCount;      // ANCOUNT: the number of answers
    private short nsCount;      // NSCOUNT: the number of authorities
    private short arCount;      // ARCOUNT: the number of additionals


    /**
     * read the header from an input stream
     * (we'll use a ByteArrayInputStream but we will only use the basic read methods of input stream to read 1 byte, or to fill in a byte array, so we'll be generic)
     * @param inputStream
     * @return DNSHeader
     */
    public static DNSHeader decodeHeader(InputStream inputStream) throws IOException {
        DNSHeader dnsHeader = new DNSHeader();
        dnsHeader.id = inputStream.readNBytes(2);
        dnsHeader.flags = inputStream.readNBytes(2);
        dnsHeader.qr = (byte)(dnsHeader.flags[0] >> 7 & 1);
        dnsHeader.opcode = (byte)(dnsHeader.flags[0] >> 3 & 0xf);
        dnsHeader.aa = (byte)(dnsHeader.flags[0] >> 2 & 1);
        dnsHeader.tc = (byte)(dnsHeader.flags[0] >> 1 & 1);
        dnsHeader.rd = (byte)(dnsHeader.flags[0] & 1);
        dnsHeader.ra = (byte)(dnsHeader.flags[1] >> 7 & 1);
        dnsHeader.z = (byte)(dnsHeader.flags[1] >> 6 & 1);
        dnsHeader.ad = (byte)(dnsHeader.flags[1] >> 5 & 1);
        dnsHeader.cd = (byte)(dnsHeader.flags[1] >> 4 & 1);
        dnsHeader.rCode = (byte)(dnsHeader.flags[1] & 0xf);
        dnsHeader.qdCount = new BigInteger(inputStream.readNBytes(2)).shortValue();
        dnsHeader.anCount = new BigInteger(inputStream.readNBytes(2)).shortValue();
        dnsHeader.nsCount = new BigInteger(inputStream.readNBytes(2)).shortValue();
        dnsHeader.arCount = new BigInteger(inputStream.readNBytes(2)).shortValue();

        return dnsHeader;
    }

    /**
     * This will create the header for the response. It will copy some fields from the request
     * @param request
     * @param response
     * @return DNSHeader
     */
    public static DNSHeader buildHeaderForResponse(DNSMessage request, DNSMessage response){
        DNSHeader dnsHeader = request.getHeader();
        dnsHeader.id = request.getHeader().id;
        dnsHeader.qr = 1;
        dnsHeader.opcode = request.getHeader().opcode;
        dnsHeader.aa = request.getHeader().aa;
        dnsHeader.tc = request.getHeader().tc;
        dnsHeader.rd = request.getHeader().rd;
        dnsHeader.ra = 1;
        dnsHeader.z = request.getHeader().z;
        dnsHeader.ad = 1;
        dnsHeader.cd = request.getHeader().cd;
        dnsHeader.rCode = request.getHeader().rCode;
        dnsHeader.qdCount = request.getHeader().qdCount ;
        dnsHeader.anCount = 1;
        dnsHeader.nsCount = request.getHeader().nsCount;
        dnsHeader.arCount = request.getHeader().arCount;

        return dnsHeader;
    }

    /**
     * encode the header to bytes to be sent back to the client
     * The OutputStream interface has methods to write a single byte or an array of bytes
     * @param outputStream
     */
    public void writeBytes(ByteArrayOutputStream outputStream) throws IOException {
        byte newQRByte = (byte)((qr<<7)|(opcode<<3)|(aa<<2)|(tc<<1)|(rd));
        byte newRAByte = (byte)((ra<<7)|(z<<6)|(rCode));

        outputStream.write(id);
        outputStream.write(newQRByte);
        outputStream.write(newRAByte);
        outputStream.write(DNSMessage.shortToByteArr(qdCount));
        outputStream.write(DNSMessage.shortToByteArr(anCount));
        outputStream.write(DNSMessage.shortToByteArr(nsCount));
        outputStream.write(DNSMessage.shortToByteArr(arCount));
    }

    /**
     * Return a human readable string version of a header object
     * A reasonable implementation can be autogenerated by your IDE
     * @return String
     */
    @Override
    public String toString(){
        String result = "DNSHeader{" +
                        "id=" + Arrays.toString(id) +
                        ", qr=" + qr +
                        ", op_code=" + opcode +
                        ", aa=" + aa +
                        ", tc=" + tc +
                        ", rd=" + rd +
                        ", ra=" + ra +
                        ", z=" + z +
                        ", ad=" + ad +
                        ", cd=" + cd +
                        ", rCode=" + rCode +
                        ", qdCount=" + qdCount +
                        ", anCount=" + anCount +
                        ", nsCount=" + nsCount +
                        ", arCount=" + arCount +
                        '}';
        return result;
    }

    public short getAnCount() {
        return anCount;
    }
    public byte getRCode() {
        return rCode;
    }
}
