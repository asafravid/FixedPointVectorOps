
#include <iostream>
#include <immintrin.h>
#include <string.h> // memcpy
#include <x86intrin.h>

void _mm256_print_epi8(__m256i );
void _mm256_print_epi16(__m256i );
void _mm256_mul_epi8(__m256i , __m256i , __m256i* , __m256i* );

template <class T> inline char GetChar(T value, size_t index)
{
    return ((char*)&value)[index];
}

#define AS_CHAR(a) char(a)

#define AS_2CHARS(a) \
    GetChar(int16_t(a), 0), GetChar(int16_t(a), 1)

#define AS_4CHARS(a) \
    GetChar(int32_t(a), 0), GetChar(int32_t(a), 1), \
    GetChar(int32_t(a), 2), GetChar(int32_t(a), 3)

#define _MM_SETR_EPI8(byte15, byte14, byte13, byte12, byte11, byte10, byte9, byte8, byte7, byte6, byte5, byte4, byte3, byte2, byte1, byte0) \
    {AS_CHAR(byte0), AS_CHAR(byte1), AS_CHAR(byte2), AS_CHAR(byte3), \
     AS_CHAR(byte4), AS_CHAR(byte5), AS_CHAR(byte6), AS_CHAR(byte7), \
     AS_CHAR(byte8), AS_CHAR(byte9), AS_CHAR(byte10),AS_CHAR(byte11), \
     AS_CHAR(byte12),AS_CHAR(byte13),AS_CHAR(byte14),AS_CHAR(byte15)}

#define _MM_SETR_EPI16(word7, word6, word5, word4, word3, word2, word1, word0) \
    {AS_2CHARS(word0), AS_2CHARS(word1), AS_2CHARS(word2), AS_2CHARS(word3), \
     AS_2CHARS(word4), AS_2CHARS(word5), AS_2CHARS(word6), AS_2CHARS(word7)}

#define _MM_SETR_EPI32(dword3, dword2, dword1, dword0) \
    {AS_4CHARS(dword0), AS_4CHARS(dword1), AS_4CHARS(dword2), AS_4CHARS(dword3)}   

void print_m128i_16x8(__m128i* to_print, const char* name) {
    uint8_t val[16];
    memcpy(val, to_print, sizeof(val));
    printf("%s: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \n", name, val[15], val[14], val[13], val[12], val[11], val[10], val[9], val[8], val[7], val[6], val[5], val[4], val[3], val[2], val[1], val[0]);
}

void print_m128i_8x16(__m128i* to_print, const char* name) {
    uint16_t val[8];
    memcpy(val, to_print, sizeof(val));
    printf("%s: 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x \n", name, val[7], val[6], val[5], val[4], val[3], val[2], val[1], val[0]);
}

void print_m128i_4x32(__m128i* to_print, const char* name) {
    uint32_t val[4];
    memcpy(val, to_print, sizeof(val));
    printf("%s: 0x%08x 0x%08x 0x%08x 0x%08x \n", name, val[3], val[2], val[1], val[0]);
}


int main()
{
    // Vector of 16 8bit elements: representing 8 8b complex elements
    // (real8b, imag8b), (real8b, imag8b), (real8b, imag8b), (real8b, imag8b) (real8b, imag8b), (real8b, imag8b), (real8b, imag8b), (real8b, imag8b)
    {
        __m128i r16x8 = _mm_setr_epi8(-1, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 9, 8, 7, 6, 5, 4, 3, 2, 1); //_MM_SETR_EPI8(-1, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 9, 8, 7, 6, 5, 4, 3, 2, 1);   // vector of real parts// vector of real parts
        __m128i j16x8 = _mm_setr_epi8(-1, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 9, 8, 7, 6, 5, 4, 3, 2, 1);   // vector of imag parts// vector of imag parts
        __m128i c16x8 = _mm_add_epi8(r16x8, j16x8);                                                   // c = r - i*j  

        print_m128i_16x8(&r16x8, "r16x8");
        print_m128i_16x8(&j16x8, "j16x8");
        print_m128i_16x8(&c16x8, "c16x8");

        __m128i vc8x8_a = _mm_setr_epi8( 8,  8,   7,   7,   6,   6,   5,   5,   4,   4,   3,   3,   2,   2, 1, 1); // vector of complex <tuple>(MSBimag, LSBreal)// vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc8x8_b = _mm_setr_epi8(-1, -1, 0xf, 0xf, 0xe, 0xe, 0xd, 0xd, 0xc, 0xc, 0xb, 0xb, 0xa, 0xa, 9, 9); // vector of complex <tuple>(MSBimag, LSBreal)// vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc8x8_c = _mm_add_epi8(vc8x8_a, vc8x8_b);                                                          // c = a + b

        print_m128i_16x8(&vc8x8_a, "vc8x8_a");
        print_m128i_16x8(&vc8x8_b, "vc8x8_b");
        print_m128i_16x8(&vc8x8_c, "vc8x8_c");
    }
/*
    {
        char a0[32] = {1, 2, 3, -4, 5, 6, 7, 8, 9, -10, 11,  12, 13, 14, 15, 16, 17, 18,  19, 20, 21, 22, 23, -24, 25, 26, 27, 28, 29, 30, 31, 32};
        char a1[32] = {2, 3, 4,  5, 6, 7, 8, 9, 10, 11, 12, -13, 14, 15, 16, 17, 18, 19, -20, 21, 22, 23, 24, -25, 26, 27, 28, 29, 30, 31, 32, 33};

        __m256i v0_32x8s = _mm256_loadu_si256((__m256i*) &a0[0]);
        __m256i v1_32x8s = _mm256_loadu_si256((__m256i*) &a1[0]);

        __m256i r0, r1;//for 16 bit results

        _mm256_mul_epi8(v0_32x8s, v1_32x8s, &r0, &r1);

        printf("\nv0_32x8s = ");_mm256_print_epi8(v0_32x8s);
        printf("\nv1_32x8s = ");_mm256_print_epi8(v1_32x8s);
        printf("\nr0 = ");_mm256_print_epi16(r0);
        printf("\nr1 = ");_mm256_print_epi16(r1);
        printf("\nfinished\n");
    }*/
    // Packet 8 16bits: representing 4 16b complex elements
    // (real16b, imag16b), (real16b, imag16b), (real16b, imag16b), (real16b, imag16b)
    {
        __m128i r8x16 = _mm_setr_epi16(6, 5, 4, 3, 2, 1, 0, -1); // vector of real parts
        __m128i j8x16 = _mm_setr_epi16(6, 5, 4, 3, 2, 1, 0, -1); // vector of imag parts
        __m128i c8x16 = _mm_add_epi16(r8x16, j8x16);             // c = r - i*j

        print_m128i_8x16(&r8x16, "r8x16");
        print_m128i_8x16(&j8x16, "j8x16");
        print_m128i_8x16(&c8x16, "c8x16");

        __m128i c8x16lo = _mm_mullo_epi16(r8x16, j8x16); // _mm_mullo_epi16(x, y): treats x and y as a vector of 16-bit signed integers, multiplies each pair of integers, and truncates (stores low 16bits) the results to 16 bits.
        __m128i c8x16hi = _mm_mulhi_epi16(r8x16, j8x16); // _mm_mulhi_epi16(x, y): treats x and y as a vector of 16-bit signed integers, multiplies each pair of integers to get a 32-bit integer, then returns the top 16 bits of each 32-bit integer result.

        print_m128i_8x16(&c8x16lo, "c8x16lo");
        print_m128i_8x16(&c8x16hi, "c8x16hi");

        __m128i vc4x16_a = _mm_setr_epi16(2, 2, 1, 1, 0, 0, -1, -1); // vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc4x16_b = _mm_setr_epi16(6, 6, 5, 5, 4, 4,  3,  3); // vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc4x16_c = _mm_add_epi16(vc4x16_a, vc4x16_b);        // c = a + b

        print_m128i_8x16(&vc4x16_a, "vc4x16_a");
        print_m128i_8x16(&vc4x16_b, "vc4x16_b");
        print_m128i_8x16(&vc4x16_c, "vc4x16_c");

        __m128i vc8x16lo = _mm_mullo_epi16(vc4x16_a, vc4x16_b); // _mm_mullo_epi16(x, y): treats x and y as a vector of 16-bit signed integers, multiplies each pair of integers, and truncates (stores low 16bits) the results to 16 bits.
        __m128i vc8x16hi = _mm_mulhi_epi16(vc4x16_a, vc4x16_b); // _mm_mulhi_epi16(x, y): treats x and y as a vector of 16-bit signed integers, multiplies each pair of integers to get a 32-bit integer, then returns the top 16 bits of each 32-bit integer result.

        print_m128i_8x16(&vc8x16lo, "vc8x16lo"); // c = real{a}*real{b} - i*(imag{a}*imag{b})
        print_m128i_8x16(&vc8x16hi, "vc8x16hi");
    }

    // Packetd 4 32bits: representing 2 32b complex elements
    // 2 32b complex (real32b, imag32b), (real32b, imag32b)
    {
        __m128i r4x32    = _mm_setr_epi32(1, 0, -1, -2); // vector of real parts
        __m128i j4x32    = _mm_setr_epi32(1, 0, -1, -2); // vector of imag parts
        __m128i c4x32    = _mm_add_epi32(r4x32, j4x32);  // c = r - i*j

        print_m128i_4x32(&r4x32, "r4x32");
        print_m128i_4x32(&j4x32, "j4x32");
        print_m128i_4x32(&c4x32, "c4x32");

        __m128i vc2x32_a = _mm_setr_epi32(-1, -1, -2, -2);    // vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc2x32_b = _mm_setr_epi32( 1,  1,  0,  0);    // vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc2x32_c = _mm_add_epi32(vc2x32_a, vc2x32_b); // c = a + b

        print_m128i_4x32(&vc2x32_a, "vc2x32_a");
        print_m128i_4x32(&vc2x32_b, "vc2x32_b");
        print_m128i_4x32(&vc2x32_c, "vc2x32_c");
    }
}

/*

Output on Console:
==================
r16x8: 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f 0xff 
j16x8: 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f 0xff 
c16x8: 0x02 0x04 0x06 0x08 0x0a 0x0c 0x0e 0x10 0x12 0x14 0x16 0x18 0x1a 0x1c 0x1e 0xfe 
vc8x8_a: 0x01 0x01 0x02 0x02 0x03 0x03 0x04 0x04 0x05 0x05 0x06 0x06 0x07 0x07 0x08 0x08 
vc8x8_b: 0x09 0x09 0x0a 0x0a 0x0b 0x0b 0x0c 0x0c 0x0d 0x0d 0x0e 0x0e 0x0f 0x0f 0xff 0xff 
vc8x8_c: 0x0a 0x0a 0x0c 0x0c 0x0e 0x0e 0x10 0x10 0x12 0x12 0x14 0x14 0x16 0x16 0x07 0x07 
r8x16: 0xffff 0x0000 0x0001 0x0002 0x0003 0x0004 0x0005 0x0006 
j8x16: 0xffff 0x0000 0x0001 0x0002 0x0003 0x0004 0x0005 0x0006 
c8x16: 0xfffe 0x0000 0x0002 0x0004 0x0006 0x0008 0x000a 0x000c 
c8x16lo: 0x0001 0x0000 0x0001 0x0004 0x0009 0x0010 0x0019 0x0024 
c8x16hi: 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 
vc4x16_a: 0xffff 0xffff 0x0000 0x0000 0x0001 0x0001 0x0002 0x0002 
vc4x16_b: 0x0003 0x0003 0x0004 0x0004 0x0005 0x0005 0x0006 0x0006 
vc4x16_c: 0x0002 0x0002 0x0004 0x0004 0x0006 0x0006 0x0008 0x0008 
vc8x16lo: 0xfffd 0xfffd 0x0000 0x0000 0x0005 0x0005 0x000c 0x000c 
vc8x16hi: 0xffff 0xffff 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 
r4x32: 0xfffffffe 0xffffffff 0x00000000 0x00000001 
j4x32: 0xfffffffe 0xffffffff 0x00000000 0x00000001 
c4x32: 0xfffffffc 0xfffffffe 0x00000000 0x00000002 
vc2x32_a: 0xfffffffe 0xfffffffe 0xffffffff 0xffffffff 
vc2x32_b: 0x00000000 0x00000000 0x00000001 0x00000001 
vc2x32_c: 0xfffffffe 0xfffffffe 0x00000000 0x00000000 

*/
