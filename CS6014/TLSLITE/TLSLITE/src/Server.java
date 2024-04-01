import java.io.*;
import java.math.BigInteger;
import java.net.*;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.security.spec.PKCS8EncodedKeySpec;
import java.util.Base64;
import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class Server {
    private static final int PORT = 9000;
    public static Certificate CACertificate;
    public static Certificate serverCertificate;
    public static void makeCACertificate() throws Exception {
        FileInputStream CACertificateFiles = new FileInputStream(KeyGeneration.CA_CERTIFICATE_FILE);
        CertificateFactory CACertificateFactory = CertificateFactory.getInstance("X.509");
        CACertificate =CACertificateFactory.generateCertificate(CACertificateFiles);
        CACertificateFiles.close();
    }

    public static void makeServerCertificate() throws Exception {
        FileInputStream serverCertificateFiles = new FileInputStream(KeyGeneration.SERVER_CERTIFICATE_FILE);
        CertificateFactory cf = CertificateFactory.getInstance("X.509");
        serverCertificate = cf.generateCertificate(serverCertificateFiles);
        serverCertificateFiles.close();
    }

    public static void main(String[] args) throws Exception {

        makeCACertificate();
        makeServerCertificate();
        FileInputStream serverPrivateKeyFile = new FileInputStream(KeyGeneration.SERVER_PRIVATE_KEY_FILE);
        byte[] keyBytes = new byte[serverPrivateKeyFile.available()];
        serverPrivateKeyFile.read(keyBytes);
        serverPrivateKeyFile.close();
        KeyFactory keyFactory = KeyFactory.getInstance("RSA");
        PKCS8EncodedKeySpec spec = new PKCS8EncodedKeySpec(keyBytes);
        PrivateKey serverPrivateKey = keyFactory.generatePrivate(spec);
        SecretKeySpec key = new SecretKeySpec(keyBytes, "RSA");


        ServerSocket serverSocket = new ServerSocket(PORT);
        System.out.println("Waiting for client");
        Socket client = serverSocket.accept();

        //create input and output stream
        ObjectOutputStream output = new ObjectOutputStream(client.getOutputStream());
        ObjectInputStream input = new ObjectInputStream(client.getInputStream());
        byte[] nonce = (byte[]) input.readObject();


        //server sends the server certificate, serverDHPublicKey and a signed Diffie-Hellman public key as Enc(serverRSAPriv, serveDHPub)
        //RSA certificate
        output.writeObject(serverCertificate);
        //Generate server's Diffie-Hellman key pair
        BigInteger[] serverDHKeyPair = KeyGeneration.generateKeyPair();
        BigInteger serverDHPrivateKP = serverDHKeyPair[0];
        BigInteger serverDHPublicKP = serverDHKeyPair[1];
        output.writeObject(serverDHPublicKP);

        //sign serverDHPublickKey
        Cipher serverDHPublicKey = Cipher.getInstance("RSA/ECB/PKCS1Padding");
        serverDHPublicKey.init(Cipher.ENCRYPT_MODE,serverPrivateKey);
        byte[] DHPublicServer = serverDHPublicKey.doFinal(serverDHPublicKP.toByteArray());
        output.writeObject(DHPublicServer);
        output.flush();

        //read clients cert and publickey and verify
        Certificate clientCertificate = (Certificate) input.readObject();
        BigInteger clientDHPublicKey = (BigInteger) input.readObject();
        byte[] signedClientDHPublicKey =(byte[]) input.readObject();
        clientCertificate.verify(CACertificate.getPublicKey());
        System.out.println("Certification verified.");

        //generate the shared secrete
        byte[] sharedSecret = KeyGeneration.generateShareKey(clientDHPublicKey,serverDHPrivateKP);

        //session keys using HKDF
        KeyGeneration.makeSecretKeys(nonce,sharedSecret);
        //write hmac history
        byte[] hmac = KeyGeneration.calculateHmac(KeyGeneration.serverMac,nonce,serverCertificate.getEncoded(),serverDHPublicKP.toByteArray(),DHPublicServer,clientCertificate.getEncoded(),clientDHPublicKey.toByteArray(),signedClientDHPublicKey);
        output.writeObject(hmac);
        output.flush();
        //read history
        byte[] receivedHmac = (byte[]) input.readObject();


        byte[] encryptedMessageBytes = (byte[])input.readObject();
        byte[] decryptedMessageBytes = KeyGeneration.decryptMessage(KeyGeneration.clientEncrypt,encryptedMessageBytes);

        byte[] receivedMessage = new byte[decryptedMessageBytes.length - 32];
        byte[] receivedHmac2 = new byte[32];
        System.arraycopy(decryptedMessageBytes, 0, receivedMessage, 0, receivedMessage.length);
        System.arraycopy(decryptedMessageBytes, receivedMessage.length, receivedHmac2, 0, 32);
        //print example message from client
        System.out.println(new String(receivedMessage,"UTF-8"));
    }
}