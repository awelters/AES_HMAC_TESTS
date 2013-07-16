#include <AESLib.h>
#include <MemoryFree.h>
#include "sha1.h"

//160-bit hmac key recommended.  20 bytes
uint8_t hmacKey[]={
   0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
   0x40,0x41,0x42,0x43
};

//key must be 128bits or 16 bytes, 0-256
uint8_t aesKey[]={
  0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5, 4,3,2,1,0,1,2,3,4,5,6,7,8,9,0,1
};

void printHash(uint8_t* hash) {
  int i;
  for (i=0; i<20; i++) {
    Serial.print("0123456789abcdef"[hash[i]>>4]);
    Serial.print("0123456789abcdef"[hash[i]&0xf]);
  }
  Serial.println();
}

void setup()
{
    Serial.begin(9600);
    Serial.println("---AES & HMAC-SHA1 Test:  START---");
    Serial.print("Memory: ");
    Serial.println(freeMemory());
    
    //data must be mod 16 bytes
    char data[256] = "0123456789012345"; //16 chars == 16 bytes
    unsigned long m1;
    unsigned long m0 = millis();
    
    aes128_enc_single(aesKey, data);
    Sha1.initHmac(hmacKey,20);
    Sha1.print(data);
    Sha1.resultHmac();
    
    m1 = millis();
    
    Serial.print("Memory: ");
    Serial.println(freeMemory());
    
    Serial.print("Time in millis to encode & hmac: ");
    Serial.println((m1 - m0));
    
    Sha1.initHmac(hmacKey,20);
    Sha1.print(data);
    printHash(Sha1.resultHmac());
    
    char data1[17] = "5123456789012345";
    
    Sha1.initHmac(hmacKey,20);
    aes128_enc_single(aesKey, data1);
    Sha1.print(data1);
    printHash(Sha1.resultHmac());
    
    char data2[17] = "0123456789012345";
    
    Sha1.initHmac(hmacKey,20);
    aes128_enc_single(aesKey, data2);
    Sha1.print(data2);
    printHash(Sha1.resultHmac());
    
    Serial.println("---AES & HMAC-SHA1 Test:  END---");
}

void loop()
{
   
}
