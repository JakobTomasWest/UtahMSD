import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class DNSHeader {
    private short transactionId;
    private short flags;
    private short questionCount;
    private short answerCount;
    private short authorityCount;
    private short additionalCount;


    public short getTransactionId() {
        return transactionId;
    }

    public short getFlags() {
        return flags;
    }

    public short getQuestionCount() {
        return questionCount;
    }

    public short getAnswerCount() {
        return answerCount;
    }

    public short getAuthorityCount() {
        return authorityCount;
    }

    public short getAdditionalCount() {
        return additionalCount;
    }

    // constructor
    private DNSHeader(short transactionId, short flags, short questionCount,
                      short answerCount, short authorityCount, short additionalCount) {
        this.transactionId = transactionId;
        this.flags = flags;
        this.questionCount = questionCount;
        this.answerCount = answerCount;
        this.authorityCount = authorityCount;
        this.additionalCount = additionalCount;
    }


    //read the header from an input stream
    // (we'll use a ByteArrayInputStream
    // but we will only use the basic read methods of input stream to read 1 byte,
    // or to fill in a byte array, so we'll be generic).
    public static DNSHeader decodeHeader(InputStream inputStream) throws IOException {
        short transactionId = (short) ((inputStream.read() << 8) | inputStream.read());
        short flags = (short) ((inputStream.read() << 8) | inputStream.read());
        short questionCount = (short) ((inputStream.read() << 8) | inputStream.read());
        short answerCount = (short) ((inputStream.read() << 8) | inputStream.read());
        short authorityCount = (short) ((inputStream.read() << 8) | inputStream.read());
        short additionalCount = (short) ((inputStream.read() << 8) | inputStream.read());

        return new DNSHeader(transactionId, flags, questionCount, answerCount, authorityCount, additionalCount);
    }




    // This will create the header for the response. It will copy some fields from the request. This will deliver a response to the client
    // from its original DNS query
    public static DNSHeader buildHeaderForResponse(DNSMessage request, DNSMessage response) {
        // Take fields from request message and construct DNSHeader for response
        // In order for us to build as reponse we need to get the header form the message
        DNSHeader requestHeader = request.getHeader();
        short transactionId = requestHeader.transactionId;
        // We only care about the QR bit in flags, 0 indicates a DNS query message and 1 is a DNS response message
        short flags = (short) 0x8000;
        short questionCount = requestHeader.questionCount;
        short answerCount;
        //return null if the query doesn't have the information, if there was no information, return an empty array
        if(response.getAnswers() != null){
            answerCount = (short) response.getAnswers().length;
        } else {
            answerCount = 0;
        }
        short authorityCount = 0;
        short additionalCount = 0;


        return new DNSHeader(transactionId, flags, questionCount, answerCount, authorityCount, additionalCount);
    }
    // Encode the header to bytes to be sent back to the client.
    // The OutputStream interface has methods to write a single byte or an array of bytes.
    public void writeBytes(OutputStream outputStream) throws IOException {
        // Write each field of the DNS header to the output stream in order
        writeShort(outputStream, transactionId);
        writeShort(outputStream, flags);
        writeShort(outputStream, questionCount);
        writeShort(outputStream, answerCount);
        writeShort(outputStream, authorityCount);
        writeShort(outputStream, additionalCount);
    }
    private void writeShort(OutputStream outputStream, short value) throws IOException {
        outputStream.write((value >> 8) & 0xFF); // High byte
        outputStream.write(value & 0xFF);       // Low byte
    }
    // Return a human-readable string version of a header object.

    @Override
    public String toString() {
        return "DNSHeader{" +
                "transactionId=" + transactionId +
                ", flags=" + flags +
                ", questionCount=" + questionCount +
                ", answerCount=" + answerCount +
                ", authorityCount=" + authorityCount +
                ", additionalCount=" + additionalCount +
                '}';
    }
}
