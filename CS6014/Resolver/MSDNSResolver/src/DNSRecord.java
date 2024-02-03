import java.io.*;
import java.util.Arrays;
import java.util.Calendar;
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

    private long timeStamp;

    // Constructor to initialize the fields
    public DNSRecord(String domainName, int type, int recordClass, int ttl, byte[] resourceData) {
        this.recordDomainName = domainName;
        this.recordType = type;
        this.recordClass = recordClass;
        this.ttl = ttl;
        this.resourceData = resourceData;
        this.creationDate = Calendar.getInstance().getTime();
    }




    public static DNSRecord decodeRecord(InputStream inputStream, DNSMessage message) throws IOException {
//        DNSRecord record = new DNSRecord();
//        DataInputStream dataInputStream = new DataInputStream(inputStream);
        // read and parse the domain name
        String[] domainNameParts = message.readDomainName(inputStream);
        String domainName = String.join(".", domainNameParts);
        System.out.println(domainName);
        // read the 2 bytes for type -- Address (A) IPv6 (AAAA)
        int type = (inputStream.read() << 8) + inputStream.read();
        System.out.println(type);
        // read the 2 bytes for class -- Probably the Internet IN
//        short type = dataInputStream.readShort();
        int recordClass = (inputStream.read() << 8) + inputStream.read();
//        short recordClass = dataInputStream.readShort();
        // read the next 4 bytes for the time-to-live
        System.out.println("recordclass: " + recordClass);
        int ttl = (inputStream.read() << 24) + (inputStream.read() << 16) + (inputStream.read() << 8) + inputStream.read();
//        int ttl = dataInputStream.readInt();
        // read the 2 byes for data length
        int dataLength =  (inputStream.read() << 8) + inputStream.read();
//        int dataLength = dataInputStream.readShort();

        // read the resource data based on dataLength --Rdata
        byte[] resourceData = new byte[dataLength];
        for(int i =0; i <dataLength; i++){
            resourceData[i]= (byte) (inputStream.read() & 0xFF);
        }
//        record.creationDate = new Date();
//        record .timeStamp = record.creationDate.getTime();
        // create and return a new DNSRecord object with the parsed data
//        return record;
        System.out.println("decode record!!!:" + new DNSRecord(domainName,type,recordClass,ttl,resourceData));
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
        DataOutputStream dataToClient = new DataOutputStream(byteArrayOutputStream);
        DNSMessage.writeDomainName(byteArrayOutputStream, domainNameLocation, recordDomainName.split("\\."));

        // write the two bytes for the type
//        byteArrayOutputStream.write((recordType >> 8) & 0xFF);
//        byteArrayOutputStream.write(recordType & 0xFF);
//        // ...
//        byteArrayOutputStream.write((recordClass >>8) & 0xFF);
//        byteArrayOutputStream.write( recordClass & 0xFF);
//        byteArrayOutputStream.write((ttl >> 24) & 0xFF);
//        byteArrayOutputStream.write((ttl >> 16) & 0xFF);
//        byteArrayOutputStream.write((ttl >> 8) & 0xFF);
//        byteArrayOutputStream.write(ttl & 0xFF);
//        byteArrayOutputStream.write((resourceData.length >> 8) & 0xFF);
//        byteArrayOutputStream.write(resourceData.length & 0xFF);
//        byteArrayOutputStream.write(resourceData);

        dataToClient.writeShort(recordType);
        dataToClient.writeShort(recordClass);
        dataToClient.writeInt(ttl); // write TTL as an integer
        dataToClient.writeShort(resourceData.length); // write the length of resource data
        dataToClient.write(resourceData);

        dataToClient.flush();
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

}
