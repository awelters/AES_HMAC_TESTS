/*
 * siphash_2_4_asm.S

 Supporting assembler routines for SipHash for 8bit Atmel processors
 
 see https://131002.net/siphash/ for details of algorithm
 
 (c)2013 Forward Computing and Control Pty. Ltd. 
 www.forward.com.au
 This code may be freely used for both private and commercial use.
 Provide this copyright is maintained.
 */ 

 #include <avr/io.h>  

loopCounter = 18 // R18 used for loop counting
tmp_a = 19
tmp_b = 20

// Arguments - allocated left to right, r25 to r8
// Call-used registers (X r26-r27, Z r30-r31) use freely in asm, Y used for frame pointer 
// Call-saved registers (r2-r17, r28-r29)  Y used for frame pointer asm needs to save/restore these

 ;******************** MAIN (START) *******************************
// call with R25,R24 a,  R23,R22 b
// result b = b+a
//  uses R18, R19,R20, R26,R27,R30,R31
.global add64le
add64le:
  push loopCounter
  push tmp_a
  push tmp_b
  // start from left and add each store in first arg
  // use XH,XL and ZH,ZL
  movw XL,R24
  adiw XL,0x08 
  movw ZL,R22
  adiw ZL,0x08 

  clc
  ldi loopCounter,8
add64le_loop:
  ld tmp_a,-X
  ld tmp_b,-Z
  adc tmp_a,tmp_b
  st X,tmp_a
  dec loopCounter
  brne add64le_loop
  pop tmp_b
  pop tmp_a
  pop loopCounter
ret  // finished

// call with R25,R24 a,  R23,R22 b
// result b = a
//  uses R18, R19, R26,R27,R30,R31
.global mov64
mov64:
  push loopCounter
  push tmp_a
  // start from left and add each store in first arg
  // use XH,XL and ZH,ZL
  movw XL,R24
  movw ZL,R22
  ldi loopCounter,8
mov64_loop:
  ld tmp_a,Z+
  st X+,tmp_a
  dec loopCounter
  brne mov64_loop
  pop tmp_a
  pop loopCounter
ret  // finished

// call with R25,R24 a,  R23,R22 b
// result b = b+a
//  uses R18, R19,R20, R26,R27,R30,R31
.global xor64
xor64:
  push loopCounter
  push tmp_a
  push tmp_b
  // start from left and add each store in first arg
  // use XH,XL and ZH,ZL
  movw XL,R24
  movw ZL,R22
  ldi loopCounter,8
xor64_loop:
  ld tmp_a,X
  ld tmp_b,Z+
  eor tmp_a,tmp_b
  st X+,tmp_a
  dec loopCounter
  brne xor64_loop
  pop tmp_b
  pop tmp_a
  pop loopCounter
ret  // finished


//extern void xor_ff(unsigned char* a);
// call with R25,R24 a
// xor ff to right most byte
.global xor_ff
xor_ff:
  push tmp_a
  push tmp_b

  movw XL,R24
  adiw XL,0x07 
  ld tmp_a,X
  ldi tmp_b, 0xff
  eor tmp_a,tmp_b  
  st X, tmp_a

  pop tmp_b
  pop tmp_a
ret

//extern void reverse64(unsigned char* a);
// call with R25,R24 a
// swap from LittleEndian to BigEndian or visa versa
.global reverse64
reverse64:
  push loopCounter
  push tmp_a
  push tmp_b

  movw XL,R24
  adiw XL,0x07 
  movw ZL,R24
  ldi loopCounter,4
reverse64_loop:
  ld tmp_a,X
  ld tmp_b,Z
  st X, tmp_b
  st Z+, tmp_a // increment Z
  sbiw XL,1; 
  dec loopCounter
  brne reverse64_loop
  pop tmp_b
  pop tmp_a
  pop loopCounter
ret

//extern void ror64(unsigned char* a, unsigned char bitToRotate);
// call with R25,R24 a,  R22 bitToRotate
.global ror64bits
ror64bits:
  // start from left and add each store in first arg
  // use XH,XL
  push loopCounter
  push tmp_a
   
ror64_loop:
  movw XL,R24
  adiw XL,0x07 
  // last byte on right;
  ld tmp_a,X
  movw XL,R24
  ldi loopCounter,8
  ror tmp_a // set carry
ror64_innerloop:
  ld tmp_a,X
  ror tmp_a
  st X+,tmp_a
  dec loopCounter
  brne ror64_innerloop
  dec r22
  brne ror64_loop
  pop tmp_a
  pop loopCounter
ret  // finished

//extern void rol64(unsigned char* a, unsigned char bitToRotate);
// call with R25,R24 a,  R22 bitToRotate
.global rol64bits
rol64bits:
  // start from left and add each store in first arg
  // use XH,XL 
  push loopCounter
  push tmp_a
  clc
rol64_loop:
  movw XL,R24
  // last byte on left;
  ld tmp_a,X
  // start at last on on right
  adiw XL,0x08  // add one more for the pre decrement below -X
  ldi loopCounter,8
  rol tmp_a // set carry
rol64_innerloop:
  ld tmp_a,-X
  rol tmp_a
  st X,tmp_a
  dec loopCounter
  brne rol64_innerloop
  dec r22
  brne rol64_loop
  pop tmp_a
  pop loopCounter
ret  // finished

//extern void rol64bytes(unsigned char* a, unsigned char bytesToRotate);
// call with R25,R24 a,  R22 bytesToRotate
.global rol64bytes
rol64bytes:
  // start from right and add each store in first arg
  // use XH,XL 
  push loopCounter
  push tmp_a
  push tmp_b
rol64b_loop:
  movw XL,R24
  movw ZL,R24
  // save left most byte;
  ld tmp_b,X+
  ldi loopCounter,7
rol64b_innerloop:
  ld tmp_a,X+
  st Z+,tmp_a
  dec loopCounter
  brne rol64b_innerloop
  // store first byte on end
  st Z,tmp_b
  dec r22
  brne rol64b_loop
  pop tmp_b
  pop tmp_a
  pop loopCounter
ret  // finished


//extern void rol_13bits(unsigned char* a);  // rotate 2 bytes left then 3 bits right
// call with R25,R24 a
// sub calls use R22 to set bits bytes to rotate
.global rol_13bits
rol_13bits:
    push R22
    ldi R22, 2
	rcall rol64bytes // rotate left 2 bytes == 16 bits
	ldi R22, 3
	rcall ror64bits  // rotate right 3 bits right => 13 bits
	pop R22
ret

//extern void rol_16bits(unsigned char* a);  // rotate 2 bytes left
// call with R25,R24 a
// sub calls use R22 to set bits bytes to rotate
.global rol_16bits
rol_16bits:
    push R22
    ldi R22, 2
	rcall rol64bytes // rotate left 2 bytes == 16 bits
	pop R22
ret

//extern void rol_32bits(unsigned char* a);  // rotate 4 bytes left
// call with R25,R24 a
// sub calls use R22 to set bits bytes to rotate
.global rol_32bits
rol_32bits:
    push R22
    ldi R22, 4
	rcall rol64bytes // rotate left 4 bytes == 32 bits
	pop R22
ret

//extern void rol_17bits(unsigned char* a);  // rotate 2 bytes left then 1 bit left
// call with R25,R24 a
// sub calls use R22 to set bits bytes to rotate
.global rol_17bits
rol_17bits:
    push R22
    ldi R22, 2
	rcall rol64bytes // rotate left 2 bytes == 16 bits
	ldi R22, 1
	rcall rol64bits  // rotate left 1 bits => 17 bits
	pop R22
ret

//extern void rol_21bits(unsigned char* a);  // rotate 3 bytes left then 3 bits right
// call with R25,R24 a
// sub calls use R22 to set bits bytes to rotate
.global rol_21bits
rol_21bits:
    push R22
    ldi R22, 3
	rcall rol64bytes // rotate left 3 bytes == 24 bits
	ldi R22, 3
	rcall ror64bits  // rotate right 3 bits  => 23 bits
	pop R22
ret


