import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

public class DNSMessage {

    private DNSHeader header;
    private DNSQuestion[] questions;
    private DNSRecord[] answers;
    private DNSRecord[] authorityRecords; // ignoring
    private DNSRecord[] additionalRecords; // ignoring

    // we will need to access these fields, so we have to create getters to allow private member variable to be accessed outside of the class
    private byte[] messageBytes;


    /*
    DNSMessage is used for first decoding the client's request, from bytes into a DNSMessage object
    It creates this object to represent a response, to client or an outgoing DNS query that we can deliver to google's DNS?
     */
    public static DNSMessage decodeMessage(byte[] bytes) throws IOException {
        ByteArrayInputStream inputStream = new ByteArrayInputStream(bytes);
        DNSMessage message = new DNSMessage();
        message.messageBytes = bytes;
        System.out.println("messageBytes initialized with length: " + message.messageBytes.length);
        // decode each part of the message and assign it to the field for the object
        message.header = DNSHeader.decodeHeader(inputStream);
        message.questions = new DNSQuestion[message.header.getQuestionCount()];
        message.answers = new DNSRecord[message.header.getAnswerCount()];
//        message.authorityRecords = new DNSRecord[message.header.getAuthorityCount()];
//        message.additionalRecords = new DNSRecord[message.header.getAdditionalCount()];
        for( int i = 0; i < message.header.getQuestionCount(); i++){
            message.questions[i] = DNSQuestion.decodeQuestion(inputStream, message);
        }
        for( int i = 0; i < message.header.getAnswerCount(); i ++){
            message.answers[i] = DNSRecord.decodeRecord(inputStream, message);
        }
//        for( int i = 0; i < message.header.getAdditionalCount(); i++){
//            message.additionalRecords[i] = DNSRecord.decodeRecord(inputStream, message);
//        }
//        for( int i = 0; i < message.header.getAuthorityCount(); i++){
//            message.additionalRecords[i] = DNSRecord.decodeRecord(inputStream, message);
//        }
        return message;

    }


    // read the pieces of a domain name starting from the current position of the input stream
    // called after the Header is handled- first bye is length, then has that length of 1 byte inputs
    public String[] readDomainName(InputStream inputStream) throws IOException {
        List<String> domainNamePieces = new ArrayList<>();
        // read the next byte for length
        int length = inputStream.read();
        //System.out.println("Length byte: " + length);
        while(length != 0){
            // Since we are looking at the domain or a repeated domain we have to check for a pointer
            if((length & 0xC0) == 0xC0) { // 0011 1111// if length that was previously mentioned is now 0x CO to FF, we have a pointer - 1100 0000 to 1111 1111 because
                // the binary values have there two largest bits as 11 --> masking will result in
                // the following two bytes will denote the offset, that value will point to the particular byte number where the domain name is found
                int secondByte = inputStream.read();    // read second byte of pointer
                // get that it's a pointer and append the second byte for the location of og domain name
                //System.out.println("Length byte: " + length + ", Second byte: " + secondByte);
                int offset = ((length & 0x3F) << 8) | secondByte;
                System.out.println("offset is " + offset);
                // add the offset to array of string to represent our pointer
                String label = readDomainNameFromOffset(offset);
                System.out.println("label is" + label);
                // add the array of pointer parts as a list to our arraylist
                domainNamePieces.add(label);
            }
            else {
                //otherwise we will just read the bytes for the domainName normally
                byte[] buffer = new byte[length]; // ie, size = 6
                int readBytes = inputStream.read(buffer); //read all 6 bytes
                if (readBytes != length) {
                    throw new IOException("We couldn't read the whole length of the DomainName");
                }
                //convert byte to string value ascii g to "g"
                String domainNamePiece = new String(buffer, "UTF-8");
                domainNamePieces.add(domainNamePiece);

            }

            length = inputStream.read();  // if all goes well the next byte read will be the next length
        }

        // list of strings --> String array
        String[] domainNameArray = domainNamePieces.toArray(new String[0]);

        return domainNameArray;
    }

    //If there is a pointer our readDomainName function will recursively call this function
    //we will read the labels from this pointer to the end of the message
    // ex., start reading from the int =12 th --- byte of the DNS message
    public String readDomainNameFromOffset(int offset) throws IOException {
        if (this.messageBytes == null || this.messageBytes.length == 0) {
            throw new IllegalStateException("messageBytes is not initialized.");
        }
//        if (offset < 0 || offset >= messageBytes.length) {
//            throw new IllegalArgumentException("Offset is out of bounds: " + offset);
//        }
        int length = this.messageBytes[offset];
        byte[] buffer = new byte[length]; // ie, size = 6
        System.arraycopy(this.messageBytes, offset + 1, buffer, 0, length);

        //convert byte to string value ascii g to "g"
        return new String(buffer, "UTF-8");
    }
//    public String[] readDomainName(int firstByte) throws IOException {
//        if (messageBytes == null) {
//            throw new IllegalStateException("DNS message bytes not set for compression processing.");
//        }
//
//        return readDomainNameFromOffset(firstByte);
//    }

    //--build a response based on the request and the answers you intend to send back.

    public static DNSMessage buildResponse(DNSMessage request, DNSRecord[] answers) {
        DNSMessage response = new DNSMessage();

        // get the answers from the DNSRecord
        response.answers = answers;
        // build response header transactionId, flags, questionCount, answerCount, authorityCount, additionalCount
        response.header = DNSHeader.buildHeaderForResponse(request, response);
        // take the questions from the request
        response.questions = request.questions;

        return response;
    }
//-------------------------------------------------------------------------------------------------------------------------------------------------//
    // get the bytes to put in a packet and send back
    public byte[] toBytes() throws IOException {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        DataOutputStream dataToClient = new DataOutputStream(byteArrayOutputStream);

        header.writeBytes(byteArrayOutputStream);
        for(DNSQuestion question : questions){
            question.writeBytes(byteArrayOutputStream, new HashMap<>());
        }
        for (DNSRecord answer : answers) {
            //if the response is not null, then write the bytes
            if (answer != null) {
                answer.writeBytes(byteArrayOutputStream, new HashMap<>());
            }
        }
        dataToClient.flush();
        return byteArrayOutputStream.toByteArray();
    }

    //If this is the first time we've seen this domain name in the packet,
    // write it using the DNS encoding (each segment of the domain prefixed with its length,
    // 0 at the end), and add it to the hash map.
    // Otherwise, write a back pointer to where the domain has been seen previously.
    public static void writeDomainName(ByteArrayOutputStream outputStream, HashMap<String, Integer> domainLocations, String[] domainPieces) throws IOException {
        DataOutputStream domainNameToClient = new DataOutputStream(outputStream);
        for (String piece : domainPieces) {
            // if the hash already contains the label we will write the pointer to the outputstream
            if (domainLocations.containsKey(piece)) {
                // write a pointer to the existing location of the domain piece
                int pointer = domainLocations.get(piece);
                // take the pointer, extract the top 8 bits, or with 1100 0000 to signify that it's a pointer
                domainNameToClient.write((pointer >> 8) | 0xC0);
                // write the second byte for location --- get rid of top 8 bits and keep bottom 8 bits
                domainNameToClient.write(pointer & 0xFF);
            } else {
                // writing an original domain name from the packet, get the string label piece and convert it to a byteArray
                byte[] pieceBytes = piece.getBytes("UTF-8");
                domainNameToClient.write(pieceBytes.length); // write first byte as the length of the label
                domainNameToClient.write(pieceBytes);        // finish the label in bytes

                // put the key string in the hash with its correlated location( current size after last byte - the label and label length)
                domainLocations.put(piece, outputStream.size() - pieceBytes.length - 1);
            }
        }
            domainNameToClient.writeByte(0); // 0 at the end to terminate label sequence
    }
    ////////


    // join the pieces of a domain name with dots ([ "utah", "edu"] -> "utah.edu" )
    public String joinDomainName(String[] pieces) {
        return String.join(".", pieces);
    }

    @Override
    public String toString() {
        return "DNSMessage{" +
                "header=" + header +
                ", questions=" + Arrays.toString(questions) +
                ", answers=" + Arrays.toString(answers) +
                ", authority= " + Arrays.toString(authorityRecords) +
                ", additionalRecords= " + Arrays.toString(additionalRecords) +
                ", messageBytes=" + Arrays.toString(messageBytes) +
                '}';
    }


    public DNSHeader getHeader() {
        return header;
    }
    public DNSQuestion[] getQuestion(){
        return questions;
    }
    public DNSRecord[] getAnswers() {
        return answers;
    }
}
