import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class DNSHeader {
    private int transactionId;
    private int flags;
    private int questionCount;
    private int answerCount;
    private int authorityCount;
    private int additionalCount;


    public int getTransactionId() {
        return transactionId;
    }

    public int getFlags() {
        return flags;
    }

    public int getQuestionCount() {
        return questionCount;
    }

    public int getAnswerCount() {
        return answerCount;
    }

    public int getAuthorityCount() {
        return authorityCount;
    }

    public int getAdditionalCount() {
        return additionalCount;
    }

    // constructor
    private DNSHeader(int transactionId, int flags, int questionCount,
                      int answerCount, int authorityCount, int additionalCount) {
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
        int transactionId = (int) ((inputStream.read() << 8) | inputStream.read());
        int flags = (int) ((inputStream.read() << 8) | inputStream.read());
        int questionCount = (int) ((inputStream.read() << 8) | inputStream.read());
        int answerCount = (int) ((inputStream.read() << 8) | inputStream.read());
        int authorityCount = (int) ((inputStream.read() << 8) | inputStream.read());
        int additionalCount = (int) ((inputStream.read() << 8) | inputStream.read());

        return new DNSHeader(transactionId, flags, questionCount, answerCount, authorityCount, additionalCount);
    }




    // This will create the header for the response. It will copy some fields from the request. This will deliver a response to the client
    // from its original DNS query
    public static DNSHeader buildHeaderForResponse(DNSMessage request, DNSRecord[] answers) {
        // Take fields from request message and construct DNSHeader for response
        // In order for us to build as reponse we need to get the header form the message
        DNSHeader requestHeader = request.getHeader();
        int transactionId = requestHeader.transactionId;
        // We only care about the QR bit in flags, 0 indicates a DNS query message and 1 is a DNS response message
        int flags =  0x8000;
        int questionCount = requestHeader.questionCount;
        int answerCount;
        answerCount =  answers.length;

        int authorityCount = 0;
        int additionalCount = 0;


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
    private void writeShort(OutputStream outputStream, int value) throws IOException {
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
