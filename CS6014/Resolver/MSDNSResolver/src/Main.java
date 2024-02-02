import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class Main {
    public static void main(String[] args) throws IOException {
        //DNS operates on port 53 on default, so we can send request using to dig to server
        int server_port = 8053;
        System.out.println("Listening at " + server_port);
        //using UDP instead of TCP - datagram
        DatagramSocket socket = new DatagramSocket(server_port);

        byte[] buffer = new byte[512];
        DatagramPacket requestPacket = new DatagramPacket(buffer, buffer.length);

        DNSCache cache = new DNSCache(); // for storing DNS records from google

        for (int count = 1; true; count++) {
            //while the server is running, recieve packets of max 512 bytes
            socket.receive(requestPacket);
            // now that we have the clients datagramPacket / DNS request we can get the data, and decode it into a DNSMessage or DNSHeader
            ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(requestPacket.getData());
//            DNSMessage request= DNSMessage.decodeMessage(requestPacket.getData());
//            System.out.println("DNSMessage request: " + request + "\n");
            DNSHeader.decodeHeader(byteArrayInputStream);

            System.out.println(count + " heard from " + requestPacket.getAddress()+ " " + requestPacket.getPort());
            for (int i = 0; i < requestPacket.getLength(); i++) {
                System.out.printf(" %x", (int)buffer[i] &0xFF);

            }

            // Decode the DNSHeader to check if it's a query
            DNSHeader header = DNSHeader.decodeHeader(byteArrayInputStream);
            if (header.getQuestionCount() > 0) {
                // Handle DNS queries
                DNSQuestion[] questions = new DNSQuestion[header.getQuestionCount()];
                for (int i = 0; i < header.getQuestionCount(); i++) {
                    questions[i] = DNSQuestion.decodeQuestion(byteArrayInputStream, new DNSMessage());
                    System.out.println("question added");
                }


                // Check if answers exist in cache and build a DNSMessage response
                DNSRecord[] answers = new DNSRecord[questions.length];
                for (int i = 0; i < questions.length; i++) {
                    DNSQuestion question = questions[i];
                    DNSRecord cachedRecord = cache.query(question);
                    if (cachedRecord != null) {
                        answers[i] = cachedRecord;

                    } else {
                        // Forward the request to Google's public DNS server (8.8.8.8)
                        DNSMessage googleResponse = forwardRequestToGoogle(requestPacket.getData());

                    }
                }

                // Build the response DNSMessage
                DNSMessage request = DNSMessage.decodeMessage(requestPacket.getData());
                System.out.println("DNSMessage request: " + request + "\n");
                DNSMessage response = DNSMessage.buildResponse(request, answers);
                System.out.println("DNSMessage response: " + response + "\n");

                // Send the response back to the client
                sendResponseToClient(socket, response, requestPacket.getAddress(), requestPacket.getPort());

            }
        }

    }
    private static DNSMessage forwardRequestToGoogle(byte[] requestData) throws IOException {
        // Create a new DatagramSocket for forwarding the request to Google
        try (DatagramSocket googleSocket = new DatagramSocket()) {
            // Set the address and port of Google's public DNS server
            InetAddress googleAddress = InetAddress.getByName("8.8.8.8");
            int googlePort = 53;

            // Create a DatagramPacket to send the request to Google
            DatagramPacket googleRequestPacket = new DatagramPacket(requestData, requestData.length, googleAddress, googlePort);
            // Send the request to Google
            googleSocket.send(googleRequestPacket);
            // Receive the response from Google
            byte[] buffer = new byte[512];
            DatagramPacket googleResponsePacket = new DatagramPacket(buffer, buffer.length);
            googleSocket.receive(googleResponsePacket);
            System.out.println("response from google: " + DNSMessage.decodeMessage(googleResponsePacket.getData()));
            // Parse and decode Google's response into a DNSMessage
            ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(googleResponsePacket.getData());

            // Create a byte array to store the data from the ByteArrayInputStream
            byte[] byteArray = new byte[byteArrayInputStream.available()];

            // Read the data from the ByteArrayInputStream into the byte array
            byteArrayInputStream.read(byteArray);
            System.out.println("DNSMessage decoded: " +DNSMessage.decodeMessage(byteArray));
            // Pass the byte array to the decodeMessage method
            return DNSMessage.decodeMessage(byteArray);

        }
    }

    // Send the DNS response back to the client
    private static void sendResponseToClient(DatagramSocket socket, DNSMessage response, InetAddress clientAddress, int clientPort) throws IOException {
        byte[] responseData = response.toBytes();
        DatagramPacket responsePacket = new DatagramPacket(responseData, responseData.length, clientAddress, clientPort);
        socket.send(responsePacket);
        System.out.println("Response sent back to client.");

    }
}

//dig request --> byte stream
//b 2b 1 20 0 1 0 0 0 0 0 1 7 65 78 61 6d 70 6c 65 3 63 6f 6d 0 0 1 0 1 0 0 29 10 0 0 0 0 0 0 0
//example               . com
//12 bytes for header tld -> additional count
//2 2b
// 2 bytes tld
//     1 20
//      2 bytes flags -----4
//           0 1
//           2 bytes question count ------6
        //      0 0
        // answer count 2 bytes -----8
                //  0 0
                // authority count 2 bytes ------ 10
                //      0 1
                // additional count 2 bytes ----- 12
//--------------------------------------------------------------------DNS header complete
                        // 7 bytes for 'example' Question byte length denoted by first byte value  '7'
                        // 65 78 61 6d 70 6c 65
                        // actual domain name before '.'
//                                              3 for com
                                              //   63 6f 6d
//                                                          //0 terminator
                                                            // 0 1 0 1 q type  & q name
    //                                                                         29 10 0 0 0 0 0 0 0
//                                                                              records - answer/ authority and additional
