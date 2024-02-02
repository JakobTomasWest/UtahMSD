import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;

public class DNSRecord {

    private String recordDomainName;
    private int recordType;             //  A, AAAA, MX
    private int recordClass;      // usually IN for internet
    private int ttl;
    // for the record
    private byte[] resourceData;
    private Date creationDate;

    // Constructor to initialize the fields
    public DNSRecord(String domainName, int type, int recordClass, int ttl, byte[] resourceData) {
        this.recordDomainName = domainName;
        this.recordType = type;
        this.recordClass = recordClass;
        this.ttl = ttl;
        this.resourceData = resourceData;
    }

    // Getters for each field
    public String getDomainName() {
        return recordDomainName;
    }

    public int getRecordType() {
        return recordType;
    }

    public int getRecordClass() {
        return recordClass;
    }

    public int getTtl() {
        return ttl;
    }

    public byte[] getResourceData() {
        return resourceData;
    }

    public static DNSRecord decodeRecord(InputStream inputStream, DNSMessage message) throws IOException {
        // read and parse the domain name
        String[] domainNameParts = message.readDomainName(inputStream);
        String domainName = String.join(".", domainNameParts);

        // read the 2 bytes for type -- Address (A) IPv6 (AAAA)
        int type = (inputStream.read() << 8) | inputStream.read();
        // read the 2 bytes for class -- Probably the Internet IN
        int recordClass = (inputStream.read() << 8) | inputStream.read();
        // read the next 4 bytes for the time-to-live
        int ttl = (inputStream.read() << 24) | (inputStream.read() << 16) | (inputStream.read() << 8) | inputStream.read();
        // read the 2 byes for data length
        int dataLength = (inputStream.read() << 8) | inputStream.read();

        // read the resource data based on dataLength --Rdata
        byte[] resourceData = new byte[dataLength];
        int actualDataLength = inputStream.read(resourceData);
        if (actualDataLength != dataLength) {
            throw new IOException("Incomplete resource data read from stream.");
        }

        // create and return a new DNSRecord object with the parsed data
        return new DNSRecord(domainName, type, recordClass, ttl, resourceData);
    }

    @Override
    public String toString() {
        return "DNSRecord{" +
                "domainName='" + recordDomainName + '\'' +
                ", type=" + recordType +
                ", recordClass=" + recordClass +
                ", ttl=" + ttl +
                ", resourceData=" + Arrays.toString(resourceData) +
                ", creationDate=" + creationDate +
                '}';
    }


    // parse bytes in order from the stream and write them
    public void writeBytes(ByteArrayOutputStream byteArrayOutputStream, HashMap<String, Integer> domainNameLocation) throws IOException {
        DNSMessage.writeDomainName(byteArrayOutputStream, domainNameLocation, recordDomainName.split("\\."));

        // write the two bytes for the type
        byteArrayOutputStream.write((recordType >> 8) & 0xFF);
        byteArrayOutputStream.write(recordType & 0xFF);
        // ...
        byteArrayOutputStream.write((recordClass >>8) & 0xFF);
        byteArrayOutputStream.write( recordClass & 0xFF);
        byteArrayOutputStream.write((ttl >> 24) & 0xFF);
        byteArrayOutputStream.write((ttl >> 16) & 0xFF);
        byteArrayOutputStream.write((ttl >> 8) & 0xFF);
        byteArrayOutputStream.write(ttl & 0xFF);
        byteArrayOutputStream.write((resourceData.length >> 8) & 0xFF);
        byteArrayOutputStream.write(resourceData.length & 0xFF);
        byteArrayOutputStream.write(resourceData);
    }

    /*
     return whether the creation date + the time to live is after the current time.
     the Date and Calendar classes are needed
     */
    public boolean isExpired(){
        //current + time to live is expiration date
        long currentTime = creationDate.getTime();
        long expirationTime = currentTime + (long)ttl*1000;
        return expirationTime < new Date().getTime();
    }

}
