/*
 * siphash_2_4_asm.h

 Supporting assembler routines for SipHash for 8bit Atmel processors
 
 see https://131002.net/siphash/ for details of algorithm
 
 (c)2013 Forward Computing and Control Pty. Ltd. 
 www.forward.com.au
 This code may be freely used for both private and commercial use.
 Provide this copyright is maintained.
 */ 

#ifndef sipHash_2_4_asm_H
#define sipHash_2_4_asm_H
extern "C" {
//                        r25:r24           r23:r22
extern void mov64(unsigned char* a, unsigned char* b);

//                        r25:r24           r23:r22
extern void add64le(unsigned char* a, unsigned char* b);

//                        r25:r24           r23:r22
extern void xor64(unsigned char* a, unsigned char* b);

//                        r25:r24           r22
extern void ror64bits(unsigned char* a, unsigned char bitToRotate);

//                        r25:r24           r22
extern void rol64bits(unsigned char* a, unsigned char bitToRotate);

//                        r25:r24           r22
extern void rol64bytes(unsigned char* a, unsigned char bytesToRotate);

//                        r25:r24
extern void rol_13bits(unsigned char* a);  // rotate 2 bytes left then 3 bits right

//                        r25:r24
extern void rol_16bits(unsigned char* a);  // rotate 2 bytes left

//                        r25:r24
extern void rol_32bits(unsigned char* a);  // rotate 4 bytes left

//                        r25:r24
extern void rol_17bits(unsigned char* a);  // rotate 2 bytes left then 1 bit left

//                        r25:r24
extern void rol_21bits(unsigned char* a);  // rotate 3 bytes left then 3 bits right

//                        r25:r24
extern void xor_ff(unsigned char* a); // xor last byte on right with 0xff

//                        r25:r24
extern void reverse64(unsigned char* a); // reverse result to match reference answers
}
#endif /* sipHash_2_4_asm_H */