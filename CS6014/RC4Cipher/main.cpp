#include <vector>
#include <iostream>
#include <string>

using namespace std;

class RC4 {
public:
    RC4(const std::vector<unsigned char>& key) {
        for (int i = 0; i < 256; ++i) {
            S[i] = i;
        }

        int j = 0;
        for (int i = 0; i < 256; ++i) {
            j = (j + S[i] + key[i % key.size()]) % 256;
            swap(S[i], S[j]);
        }
    }

    unsigned char next_byte() {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        return S[(S[i] + S[j]) % 256];
    }

    void encrypt_decrypt(vector<unsigned char>& data) {
        for (auto& byte : data) {
            byte ^= next_byte();
        }
    }

private:
    int i = 0, j = 0;
    unsigned char S[256];
};

int main() {
    //key and og message
    vector<unsigned char> key = {'S', 'e', 'c', 'r', 'e', 't'};
    vector<unsigned char> original_message = {'Y', 'o', 'u', 'r', ' ', 's', 'a', 'l', 'a', 'r', 'y', ' ', 'i', 's', ' ', '$', '1', '0', '0', '0'};

    RC4 rc4_encrypt(key);
    //copy for encryption
    vector<unsigned char> encrypted_message = original_message;
    //encrypt original message
    rc4_encrypt.encrypt_decrypt(encrypted_message);


    vector<unsigned char> altered_encrypted_message = encrypted_message; //copy of encrypted for alteration
    vector<unsigned char> target = {'$', '1', '0', '0', '0'};
    vector<unsigned char> newSalary = {'$', '9', '9', '9', '9'};

    //performing bit-flipping attack on the encrypted data
    for (size_t i = 0; i < target.size(); ++i) {
        //modifies ct using xor
        altered_encrypted_message[15 + i] ^= (target[i] ^ newSalary[i]);
    }

    //decrypting the bit-flipped encrypted message
    RC4 rc4_decrypt(key); //creating a new RC4 instance for decryption
    rc4_decrypt.encrypt_decrypt(altered_encrypted_message); //decrypt altered message


    cout << "Decrypted, altered message: ";
    for (char c : altered_encrypted_message) {
        cout << c;
    }


    return 0;
}
