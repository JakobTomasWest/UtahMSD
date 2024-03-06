#include <iostream>
#include <array>
#include <vector>
#include <algorithm> // for std::shuffle
#include <random>    // for std::default_random_engine
#include <chrono>
#include <iomanip>
using namespace std;

using Block = std::array<uint8_t, 8>;

//The key starts as an array of 8 zeros. To generate the key, do the following:
//
//for i = 0 to password length:
//  key[i mod 8] = key[i mod 8] xor password[i]
Block generateEncryptionKey(const std::string& password){
    Block key {0};
    for(size_t i = 0; i < password.length(); i++){
        //divide i with 8 and get the remainder 3%8 = 3/8 with the our remainder value 3
        key[i%8] = key[i%8] ^ password[i];
    }
    return key;
}

array<array<uint8_t, 256>, 8> generateSubstitutionTables() {
    array<array<uint8_t, 256>, 8> tables;
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < tables.size(); i++) {
        //represent the substitution as a 256-element array; a that is, byte with a value x will be substituted by array[x]
        for(int j =0; j < 256; j++){
            tables[i][j] = j;
        }
        //shuffle using Fisher-Yates
        for(int j = 255; j >= 0; j--){
            int k = rand() % (j + 1);
            //swap with random
            uint8_t temp = tables[i][j];
            tables[i][j] = tables[i][k];
            tables[i][k] = temp;
        }
    }
    return tables;
}

//initialize the algorithm's state to the input message
Block encrypt(const Block& inputMessage, const Block& key, array<array<uint8_t, 256>, 8> tables) {
    Block state = inputMessage;
    //each state is 8, 8 bit integers 0-255
    for (int round = 0; round < 16; ++round) {
        //xor the current state with the key
        for (int i = 0; i < 8; i++) {
            //take each byte and exor with key
            state[i] = state[i] ^ key[i];
        }
        //for each byte in the state, substitute that byte using the appropriate substitution table
        // (byte 0 should use table 0, byte 1 should use table 1, etc)
        for(int i = 0; i < 8; i++){
            //for 0 to 8 bytes of tables, sub byte using the correct subtable
            //if the state[3] is 46 will be will go to the [4th] table and find the 46th index and
            //take that value to store in our block index
            state[i] = tables[i][state[i]];
        }
        //rotate the entire state 1 bit to the left
        //(so, after this, byte 0 will contain contributions from bytes 0 and 1,
        // while byte 7 will contain contributions from byte 7 and byte 0).
        uint8_t lastbit = 0;
        for (int i = 0; i < 8; ++i) {
            uint8_t highestbit = state[i] >> 7;
            //shift left and append last bit --> removing highest bit
            //and give it to the next byte
            state[i] = (state[i] << 1) | lastbit;
            lastbit = highestbit;
        }
        //add final highest bit from the 8th byte to the first byte
        state[0] |= lastbit;
    }
    return state;
}


array<array<uint8_t, 256>, 8> generateReverseSubstitutionTables(const array<array<uint8_t, 256>, 8>& tables) {
    array<array<uint8_t, 256>, 8> reverseTables;
    for (size_t i = 0; i < tables.size(); i++) {
        for (size_t j = 0; j < 256; j++) {
            reverseTables[i][tables[i][j]] = j;
        }
    }
    return reverseTables;
}

Block decrypt(const Block& ciphertext, const Block& key, const array<array<uint8_t, 256>, 8>& reverseTables) {
    Block state = ciphertext;
    for (int round = 0; round < 16; round++) {
        //rotate the entire state 1 bit to the right
        uint8_t firstBit = state[7] & 1; //save the last bit of the last byte

        for (int i = 7; i > 0; i--) {
            //discard the lowest bit , take lowest bit of previous state and add it to the new states highbit pos
            state[i] = (state[i] >> 1) | (state[i - 1] << 7);
        }

        state[0] = (state[0] >> 1) | (firstBit << 7);

        //apply reverse substitution
        for (size_t i = 0; i < state.size(); ++i) {
            state[i] = reverseTables[i][state[i]];
        }

        //xor the current state with the key
        for (size_t i = 0; i < state.size(); ++i) {
            state[i] ^= key[i];
        }
    }
    return state;
}

    int main() {
    string password = "secret";
    Block key = generateEncryptionKey(password);
    auto tables = generateSubstitutionTables();
    auto reverseSubstitutionTables = generateReverseSubstitutionTables(tables);

    Block plaintext = { 'h', 'e', 'l', 'l', 'o', ' ', ' ', ' ' };
    Block ciphertext = encrypt(plaintext, key, tables);

    cout << "PlainText: ";
    for(uint8_t byte : plaintext){
//        cout << hex << setw(2) << setfill('0') << static_cast<int>(byte) << " ";
        cout << static_cast<char>(byte);
    }
    cout << endl;
    cout << "CipherText: ";
    // Output ciphertext for demonstration
    for (uint8_t byte : ciphertext) {
        cout << hex << static_cast<int>(byte) << " ";
    }

    cout << endl;
    Block decryptedTest = decrypt(ciphertext,key,reverseSubstitutionTables);

    cout << "Decrypted cipherText: ";
    for( u_int8_t byte : decryptedTest){
        cout << static_cast<char>(byte);
//        cout << hex << setw(2) << setfill('0') << static_cast<int>(byte) << " ";
    }


    return 0;
}

