import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Objects;

public class DNSQuestion {

    private String domainName; // the domain name being queried
    private int type;          // the type of the query (e.g., A, AAAA, MX)
    private int queryClass;    // the class of the query (usually IN for internet)

    // constructor -- Question include a domain name type and query calss
    public DNSQuestion(String domainName, int type, int queryClass) {
        this.domainName = domainName;
        this.type = type;
        this.queryClass = queryClass;
    }


    public String getDomainName() {
        return domainName;
    }

    public int getType() {
        return type;
    }

    public int getQueryClass() {
        return queryClass;
    }
    // read a question from the input stream. Due to compression,
    // you may have to ask the DNSMessage containing this question to read some of the fields.
    public static DNSQuestion decodeQuestion(ByteArrayInputStream inputStream, DNSMessage message) throws IOException {
        // read the domain name using DNSMessage's method
        String[] domainNameParts = message.readDomainName(inputStream);
        String domainName = String.join(".", domainNameParts);

        // put the first two bytes into the type and the next two into the queryClass
        int type = ((inputStream.read() << 8) | inputStream.read());
        int queryClass = ((inputStream.read() << 8) | inputStream.read());


        return new DNSQuestion(domainName, type, queryClass);
    }

    //Write the question bytes which will be sent to the client.
    //The hash map is used for us to compress the message
    public void writeBytes(ByteArrayOutputStream byteArrayOutputStream, HashMap<String, Integer> domainNameLocations) throws IOException {
        DataOutputStream dataToClient = new DataOutputStream(byteArrayOutputStream);
        // write the domain name with DNS message compression
        DNSMessage.writeDomainName(byteArrayOutputStream, domainNameLocations, domainName.split("\\."));

        // write the bytes
//        writeTwoBytes(byteArrayOutputStream, type);
//        writeTwoBytes(byteArrayOutputStream, queryClass);
        dataToClient.writeShort(type);
        dataToClient.writeShort(queryClass);
        dataToClient.flush();
    }
    // you have to write byte 1 at time for byteArrayOutputStream API
    private void writeTwoBytes(ByteArrayOutputStream byteArrayOutputStream, int value) {
        // get highest 8 bits
        byteArrayOutputStream.write((value >> 8) & 0xFF);
        // get lower 8 bits
        byteArrayOutputStream.write(value & 0xFF);
    }
    @Override
    public String toString() {
        return "DNSQuestion{" +
                "domainName='" + domainName + '\'' +
                ", type=" + type +
                ", queryClass=" + queryClass +
                '}';
    }
    // making the question object act as a HashMap key?
    @Override
    public int hashCode() {
        return Objects.hash(domainName, type, queryClass);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DNSQuestion that = (DNSQuestion) o;
        return type == that.type && queryClass == that.queryClass && Objects.equals(domainName, that.domainName);
    }
}
