#ifndef SipHash_2_4_H
#define SipHash_2_4_H

/*
 SipHash_2_4.h
 SipHash for 8bit Atmel processors
 
 Note: one instance sipHash is already constructed in .cpp file
 
 Usage
 unsigned char key[] PROGMEM = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
     sipHash.initFromPROGMEM(key); // initialize with key NOTE: key is stored in flash (PROGMEM)
     // use sipHash.initFromRAM(key); if key in RAM 
     for (int i=0; i<msgLen;i++) {
      sipHash.updateHash((byte)c); // update hash with each byte of msg
    }
    sipHash.finish(); // finish
    // sipHash.result then contains the 8bytes of the hash in BigEndian format

 
 see https://131002.net/siphash/ for details of algorithm
 
 (c)2013 Forward Computing and Control Pty. Ltd. 
 www.forward.com.au
 This code may be freely used for both private and commercial use.
 Provide this copyright is maintained.
 */

#include <inttypes.h>
#include <avr/pgmspace.h>

class SipHash_2_4 {
 public:
 	 SipHash_2_4(void); // This class already defines an instance sipHash, see the SipHashTest.ino
/*
**  use this init if the key is in an flash (program) memory
// Define your 'secret' 16 byte key in program memory (flash memory)
 unsigned char key[] PROGMEM = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
 // to start hashing initialize with your key
 sipHash.initFromPROGMEM(key);
 */
 void initFromPROGMEM(const unsigned char *keyPrgPtrIn);
 
 
/*
**  use this init if the key is in an RAM memory
// Define your 'secret' 16 byte key array
 unsigned char key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
 // to start hashing initialize with your key
 sipHash.initFromRAM(key);
 */
void initFromRAM(const unsigned char *key);

void updateHash(unsigned char c);
void finish();
unsigned char *result; // after finalize() always points to v0

// the state in RAM  public for debugging
unsigned char v0[8];
unsigned char v1[8];
unsigned char v2[8];
unsigned char v3[8];

private:
	
void hash(unsigned char* m);
void siphash_round();

// the local storage to bytes to hash
unsigned char m[8]; // bytes to hash
signed char m_idx;  // counts from 7 down to -1
unsigned char msg_byte_counter; // count of msg bytes % 256
// total ram 42 bytes + stack usage for calls
};
extern SipHash_2_4 sipHash; // defined in .cpp file

#endif
