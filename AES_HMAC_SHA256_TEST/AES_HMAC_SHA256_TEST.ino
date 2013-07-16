#include <AESLib.h>
#include <MemoryFree.h>
#include "sha256.h"

//256-bit hmac key recommended.  See 3.1 Keying Material here:  http://w3.antd.nist.gov/iip_pubs/draft-ietf-ipsec-ciph-sha-256-01.txt
uint8_t hmacKey0[]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10
                       ,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20
};

uint8_t hmacKey1[]={
  0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,
  0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c
};

uint8_t hmacKey[]={
  0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5, 4,3,2,1,0,1,2,3,4,5,6,7,8,9,0,1
};

void printHash(uint8_t* hash) {
  int i;
  for (i=0; i<32; i++) {
    Serial.print("0123456789abcdef"[hash[i]>>4]);
    Serial.print("0123456789abcdef"[hash[i]&0xf]);
  }
  Serial.println();
}

void setup()
{
    Serial.begin(9600);
    Serial.println("---AES & HMAC-SHA256 Test:  START---");
    Serial.print("Memory: ");
    Serial.println(freeMemory());
    
    Serial.println("Test Case #1: HMAC-SHA-256 with 3-byte input and 32-byte key: ");
    Serial.println("Expect:a21b1f5d4cf4f73a4dd939750f7a066a7f98cc131cb16a6692759021cfab8181");
    Serial.print("Result:");
    Sha256.initHmac(hmacKey0,32);
    Sha256.print("abc");
    printHash(Sha256.resultHmac());
    Serial.println();
    
    Serial.println("Test Case #8: HMAC-SHA-256 with 20-byte input and 32-byte key: ");
    Serial.println("Expect:7546af01841fc09b1ab9c3749a5f1c17d4f589668a587b2700a9c97c1193cf42");
    Serial.print("Result:");
    Sha256.initHmac(hmacKey1,32);
    Sha256.print("Test With Truncation");
    printHash(Sha256.resultHmac());
    Serial.println();
    
    //key must be 128bits or 16 bytes, 0-256
    uint8_t key[] = {hmacKey[0],hmacKey[1],hmacKey[2],hmacKey[3],hmacKey[4],hmacKey[5],hmacKey[6],hmacKey[7],hmacKey[8],hmacKey[9],hmacKey[10],hmacKey[1],hmacKey[12],hmacKey[13],hmacKey[14],hmacKey[15]};
    //data must be mod 16 bytes
    char data[256] = "0123456789012345"; //16 chars == 16 bytes
    aes128_enc_single(key, data);
    Serial.print("encrypted:");
    Serial.println(data);
    //Sha256.initHmac(hmacKey5,128);
    Sha256.initHmac(hmacKey,32);
    Sha256.print(data);
    printHash(Sha256.resultHmac());
    aes128_dec_single(key, data);
    Serial.print("decrypted:");
    Serial.println(data);
    Serial.print("Memory: ");
    Serial.println(freeMemory());
    
    unsigned long m1;
    unsigned long m0 = millis();
    aes128_enc_single(key, data);
    //Sha256.initHmac(hmacKey5,128);
    Sha256.initHmac(hmacKey,32);
    Sha256.print(data);
    Sha256.resultHmac();
    m1 = millis();
    Serial.print("Time in millis to encode & hmac: ");
    Serial.println((m1 - m0));
    Serial.println("---AES & HMAC-SHA256 Test:  END---");
}

void loop()
{
   
}
