#include <AESLib.h>
#include <MemoryFree.h>
#include "sipHash_2_4.h"
#include "HexConversionUtils.h"

/* Good practice to use different keys for confidentiality and message authentication */
/* keys must be 128bits or 16 bytes, 0-256 to prevent brute-force attacks */

//key for SipHash
unsigned char key[] PROGMEM = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

//key for AES
uint8_t aesKey[] = {0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
                               0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};

void printKey(unsigned char PROGMEM *key) {
  Serial.print(F(" Key:"));
  unsigned char tmpKey[16];
  memcpy_P(tmpKey,key,16);
  char tmp[33];
  hexToAscii((const unsigned char*)tmpKey,16,tmp,33);
  Serial.println(tmp);
}

void setup()
{
    Serial.begin(9600);
    Serial.println("---AES & SipHash Test:  START---");
    Serial.print("Memory: ");
    Serial.println(freeMemory());
    
    //data must be mod 16 bytes
    char data[256] = "0123456789012345"; //16 chars == 16 bytes
    aes128_enc_single(aesKey, data);
    Serial.print("encrypted:");
    Serial.println(data);
    
    // to start hashing initialize with your key
    sipHash.initFromPROGMEM(key);
    // for each byte in the message call updateHash()
    for (int i=0; i<16;i++) {
      sipHash.updateHash((byte)data[i]); // update hash with each byte of msg
    }
    // at the end of the message call finalize to calculate the result
    sipHash.finish(); // finish
    
    char tmp[17];
    // the unsigned char[8] variable, sipHash.result, then contains the 8 bytes of the hash in BigEndian format
    hexToAscii(sipHash.result,8,tmp,17);
    Serial.println(tmp);
    
    aes128_dec_single(aesKey, data);
    Serial.print("decrypted:");
    Serial.println(data);
    Serial.print("Memory: ");
    Serial.println(freeMemory());
    
    unsigned long m1;
    unsigned long m0 = millis();
    aes128_enc_single(aesKey, data);
    
    sipHash.initFromPROGMEM(key);
    for (int i=0; i<16;i++) {
      sipHash.updateHash((byte)data[i]);
    }
    sipHash.finish();
    
    m1 = millis();
    Serial.print("Time in millis to encode & hmac: ");
    Serial.println((m1 - m0));
    Serial.println("---AES & SipHash Test:  END---");
}

void loop()
{
   
}
