#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	

	 // Message to be encrypted (6 bytes)
	    uint8_t plaintext[] = { 'H', 'e', 'l', 'l', 'o', '!' };

	    // Key (256-bit)
	    uint8_t key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	                      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	                      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	                      0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };

	    // Nonce (64-bit)
	    uint8_t nonce[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	    // Ciphertext buffer
	    uint8_t ciphertext[6];

	    // Encrypt the message
	    salsa20_encrypt(ciphertext, plaintext, 6, key, nonce);

	    printf("Encrypted message: ");
	    for (int i = 0; i < 6; i++) {
	        printf("%02x ", ciphertext[i]);
	    }
	    printf("\n");

	    // Decrypt the message
	    salsa20_decrypt(plaintext, ciphertext, 6, key, nonce);

	    printf("Decrypted message: ");
	    for (int i = 0; i < 6; i++) {
	        printf("%c ", plaintext[i]);
	    }
	    printf("\n");


	return 0;
}




void salsa20_decrypt(uint8_t *plaintext, const uint8_t *ciphertext,
                     uint64_t length, const uint8_t *key,
                     const uint8_t *nonce)
{
    // State array
    uint32_t state[16];

    // Key schedule
    state[0] = 0x61707865;
    state[1] = 0x3320646e;
    state[2] = 0x79622d32;
    state[3] = 0x6b206574;
    for (int i = 0; i < 8; i++) {
        state[i+4] = key[i*4+0] | key[i*4+1] << 8 | key[i*4+2] << 16 | key[i*4+3] << 24;
    }

    // Nonce
    state[14] = nonce[0] | nonce[1] << 8 | nonce[2] << 16 | nonce[3] << 24;
    state[15] = nonce[4] | nonce[5] << 8 | nonce[6] << 16 | nonce[7] << 24;

    // Decryption
    while (length > 0) {
        // Generate keystream
        salsa20_round(state);

        // XOR ciphertext with keystream
        for (int i = 0; i < 64; i++) {
            if (length > 0) {
                plaintext[i] = ciphertext[i] ^ state[i%16];
                length--;
            } else {
                break;
            }
        }

        // Update state
        state[8]++;
        if (state[8] == 0) {
            state[9]++;
        }
    }
}

void salsa20_encrypt(uint8_t *ciphertext, const uint8_t *plaintext,
                     uint64_t length, const uint8_t *key,
                     const uint8_t *nonce)
{
    // State array
    uint32_t state[16];

    // Key schedule
    state[0] = 0x61707865;
    state[1] = 0x3320646e;
    state[2] = 0x79622d32;
    state[3] = 0x6b206574;
    for (int i = 0; i < 8; i++) {
        state[i+4] = key[i*4+0] | key[i*4+1] << 8 | key[i*4+2] << 16 | key[i*4+3] << 24;
    }

    // Nonce
    state[14] = nonce[0] | nonce[1] << 8 | nonce[2] << 16 | nonce[3] << 24;
    state[15] = nonce[4] | nonce[5] << 8 | nonce[6] << 16 | nonce[7] << 24;

    // Encryption
    while (length > 0) {
        // Generate keystream
        salsa20_round(state);

        // XOR plaintext with keystream
        for (int i = 0; i < 64; i++) {
            if (length > 0) {
                ciphertext[i] = plaintext[i] ^ state[i%16];
                length--;
            } else {
                break;
            }
        }

        // Update state
        state[8]++;
        if (state[8] == 0) {
            state[9]++;
        }
    }
}

