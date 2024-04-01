import java.math.BigInteger;
import java.security.SecureRandom;
import javax.crypto.Cipher;
import javax.crypto.Mac;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import java.util.Arrays;

public class KeyGeneration {
    public static final String CA_CERTIFICATE_FILE = "/Users/jakobwest/UtahMSD/CS6014/TLSLITE/CAcertificate.pem";
    public static final String SERVER_CERTIFICATE_FILE = "/Users/jakobwest/UtahMSD/CS6014/TLSLITE/CASignedServerCertificate.pem";
    public static final String SERVER_PRIVATE_KEY_FILE = "/Users/jakobwest/UtahMSD/CS6014/TLSLITE/serverPrivateKey.der";
    public static final String CLIENT_PRIVATE_KEY_FILE = "/Users/jakobwest/UtahMSD/CS6014/TLSLITE/clientPrivateKey.der";

    public static final String CLIENT_CERTIFICATE_FILE = "/Users/jakobwest/UtahMSD/CS6014/TLSLITE/CASignedClientCertificate.pem";
    public static final int PORT = 9000;

    public static byte[] serverEncrypt, serverMac, serverIV, clientEncrypt, clientMAC, clientIV;

    //Generate an encrypted message using the provided key and provided message using the Cipher-AES... MODE
    public static byte[] encryptMessage(byte[] key,byte[] message) throws Exception{
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        SecretKey secretKey = new SecretKeySpec(key,"AES");
        cipher.init(Cipher.ENCRYPT_MODE, secretKey);
        return cipher.doFinal(message);
    }

    public static byte[] decryptMessage(byte[] key ,byte[]message) throws Exception{
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        SecretKey secretKey = new SecretKeySpec(key,"AES");
        cipher.init(Cipher.DECRYPT_MODE, secretKey);
        return cipher.doFinal(message);
    }

    private static final BigInteger N = new BigInteger("FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AACAA68FFFFFFFFFFFFFFFF", 16);
    private static final BigInteger g = BigInteger.valueOf(2);
    public static byte[] generateNonce(){
        SecureRandom random = new SecureRandom();
        byte[] sectionKey = new byte[32];
        random.nextBytes(sectionKey);
        return sectionKey;
    }

    public static BigInteger[] generateKeyPair() {
        // Generate a random secret key and its corresponding public key
        SecureRandom random = new SecureRandom();
        BigInteger privateKey = new BigInteger(N.bitLength(), random);
        privateKey = privateKey.mod(N.subtract(BigInteger.ONE)).add(BigInteger.ONE);
        BigInteger publicKey = g.modPow(privateKey, N);
        return new BigInteger[]{privateKey, publicKey};
    }

    public static byte[] generateShareKey(BigInteger otherSidePublicKey, BigInteger MyOwnPrivateKey){
        return otherSidePublicKey.modPow(MyOwnPrivateKey,N).toByteArray();
    }

    public static byte[] hkdfExpand(byte[] input, String tag) throws Exception{
        //Create instance with Sha256
        Mac HMAC = Mac.getInstance("HmacSHA256");
        //Initialize HMAC with secret key spec
        SecretKeySpec spec = new SecretKeySpec(input, "RawBytes");
        HMAC.init(spec);
        //Update with bit tag
        HMAC.update(tag.getBytes());
        HMAC.update((byte)0x01);
        //Return derived HMAC output key
        return Arrays.copyOf(HMAC.doFinal(), 16);
    }

    public static void makeSecretKeys(byte[] clientNonce, byte[]sharedSecretFromDiffieHellman) throws Exception{
        Mac HMAC = Mac.getInstance("HmacSHA256");
        //Use client nonce for initial key encryption -server encryption prk w/DH
        SecretKeySpec spec = new SecretKeySpec(clientNonce, "HmacSHA256");
        HMAC.init(spec);
        HMAC.update(sharedSecretFromDiffieHellman);
        byte[] prk = HMAC.doFinal();
        //use the previous key and encryption tag to derive each key in sequence
        serverEncrypt = KeyGeneration.hkdfExpand(prk, "server encrypt");
        clientEncrypt= hkdfExpand(serverEncrypt, "client encrypt");
        serverMac = hkdfExpand(clientEncrypt, "server MAC");
        clientMAC = hkdfExpand(serverMac, "client MAC");
        serverIV = hkdfExpand(clientMAC, "server IV");
        clientIV = hkdfExpand(serverIV, "client IV");
    }

    //varargs allows us pass more than one byte array for the messages
    public static byte[] calculateHmac(byte[] key, byte[]... messages) throws Exception {
        Mac mac = Mac.getInstance("HmacSHA256");
        SecretKeySpec secretKeySpec = new SecretKeySpec(key, "HmacSHA256");
        mac.init(secretKeySpec);

        //for each array of bytes sent in the messages update the message authentication code
        for (byte[] message : messages) {
            mac.update(message);
        }
        //doFinal will process the remaining data
        return mac.doFinal();
    }

}
