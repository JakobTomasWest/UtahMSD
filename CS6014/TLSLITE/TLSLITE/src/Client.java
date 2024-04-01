import java.io.*;
import java.math.BigInteger;
import java.net.*;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.security.spec.PKCS8EncodedKeySpec;
import java.util.Arrays;
import java.util.Base64;
import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class Client {
    private static final int PORT = 9000;
    private static final String CA_CERTIFICATE_FILE = "/Users/jakobwest/UtahMSD/CS6014/TLSLITE/CAcertificate.pem";
    public static Certificate CACertificate;
    private static final String CLIENT_CERTIFICATE_FILE = "/Users/jakobwest/UtahMSD/CS6014/TLSLITE/CASignedClientCertificate.pem";
    public static Certificate clientCertificate;
    private static final String SERVER_CERTIFICATE_FILE = "/Users/jakobwest/UtahMSD/CS6014/TLSLITE/CASignedServerCertificate.pem";
    private static final String SERVER_PRIVATE_KEY_FILE = "/Users/jakobwest/UtahMSD/CS6014/TLSLITE/serverPrivateKey.der";
    private static final String CLIENT_PRIVATE_KEY_FILE = "/Users/jakobwest/UtahMSD/CS6014/TLSLITE/clientPrivateKey.der";

    public static void makeCACertificate() throws Exception {
        FileInputStream CACertificateFiles = new FileInputStream(CA_CERTIFICATE_FILE);
        CertificateFactory CACertificateFactory = CertificateFactory.getInstance("X.509");
        CACertificate = CACertificateFactory.generateCertificate(CACertificateFiles);
        CACertificateFiles.close();
    }
    public static void makeClientCertificate() throws Exception {
        FileInputStream clientCertificateFiles = new FileInputStream(CLIENT_CERTIFICATE_FILE);
        CertificateFactory clientPrivateKeyCertificateFactory = CertificateFactory.getInstance("X.509");
        clientCertificate = clientPrivateKeyCertificateFactory.generateCertificate(clientCertificateFiles);
        clientCertificateFiles.close();
    }

    public static void main(String[] args) throws Exception {
        makeCACertificate();
        makeClientCertificate();

        FileInputStream clientPrivateKeyFile = new FileInputStream(CLIENT_PRIVATE_KEY_FILE);
        byte[] privateKeyBytesClient = new byte[clientPrivateKeyFile.available()];
        //Read private key bytes into the file
        clientPrivateKeyFile.read(privateKeyBytesClient);
        clientPrivateKeyFile.close();
        KeyFactory clientKeyFactory = KeyFactory.getInstance("RSA");
        PKCS8EncodedKeySpec specClient = new PKCS8EncodedKeySpec(privateKeyBytesClient);
        PrivateKey clientPrivateKey = clientKeyFactory.generatePrivate(specClient);

        Socket socket = new Socket("localhost", KeyGeneration.PORT);
        ObjectInputStream input = new ObjectInputStream(socket.getInputStream());
        ObjectOutputStream output = new ObjectOutputStream(socket.getOutputStream());

        //send the nonce
        byte[] nonce = KeyGeneration.generateNonce();
        output.writeObject(nonce);
        output.flush();

        //Enc(serverRSAPrivate, serverDHPublicKey)
        Certificate serverCertificate = (Certificate)input.readObject();
        BigInteger serverDHPublicKey = (BigInteger) input.readObject();
        byte[] signedServerDHPublic = (byte[]) input.readObject();
        serverCertificate.verify(CACertificate.getPublicKey());
        //send client certificate
        output.writeObject(clientCertificate);
        //send  clientDHPublicKP, and signed Diffie-Hellman public key
        BigInteger[] clientDHKeyPair = KeyGeneration.generateKeyPair();
        BigInteger clientDHPrivateKP = clientDHKeyPair[0];
        BigInteger clientDHPublicKP = clientDHKeyPair[1];
        output.writeObject(clientDHPublicKP);

        // signed client public key
        Cipher clientDHPublicKey = Cipher.getInstance("RSA/ECB/PKCS1Padding");
        clientDHPublicKey.init(Cipher.ENCRYPT_MODE,clientPrivateKey);
        byte[] DHPubClient = clientDHPublicKey.doFinal(clientDHPublicKP.toByteArray());
        output.writeObject(DHPubClient);
        output.flush();

        //generate the share secret
        byte[] sharedSecret = KeyGeneration.generateShareKey(serverDHPublicKey,clientDHPrivateKP);

        //session keys using HKDF
        KeyGeneration.makeSecretKeys(nonce,sharedSecret);
        byte[] expectHMAC = KeyGeneration.calculateHmac(KeyGeneration.serverMac,nonce,serverCertificate.getEncoded(),serverDHPublicKey.toByteArray(),signedServerDHPublic,clientCertificate.getEncoded(),clientDHPublicKP.toByteArray(),DHPubClient);
        byte[] receivedHMAC = (byte[]) input.readObject();
        if (!Arrays.equals(expectHMAC,receivedHMAC)){
            throw new Exception("HMAC verification failed");
        }

        byte[]hmac = KeyGeneration.calculateHmac(KeyGeneration.clientMAC,nonce,serverDHPublicKey.toByteArray(),DHPubClient,clientCertificate.getEncoded(),clientDHPublicKP.toByteArray(),DHPubClient,nonce,serverCertificate.getEncoded(),serverDHPublicKey.toByteArray(),signedServerDHPublic,clientCertificate.getEncoded(),clientDHPublicKP.toByteArray(),DHPubClient);
//        System.out.println(hmac.length);
        output.writeObject(hmac);
        output.flush();
        System.out.println("Client handshake completed");

        String hello = "Example Message";
//        System.out.println(hello);
        byte[] message = hello.getBytes();

        byte[] hmacMessage = KeyGeneration.calculateHmac(KeyGeneration.clientMAC,message);
        byte[] combinedMessage = new byte[message.length+hmacMessage.length];
        System.arraycopy(message, 0, combinedMessage, 0, message.length);
        System.arraycopy(hmacMessage, 0, combinedMessage, message.length, hmacMessage.length);
        byte[] sendMessage = KeyGeneration.encryptMessage(KeyGeneration.clientEncrypt,combinedMessage);
        output.writeObject(sendMessage);

    }
}