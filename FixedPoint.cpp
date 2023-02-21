
#include <iostream>
#include <immintrin.h>

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
        __m128i r16x8 = _MM_SETR_EPI8(-1, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 9, 8, 7, 6, 5, 4, 3, 2, 1);   // vector of real parts// vector of real parts
        __m128i j16x8 = _MM_SETR_EPI8(-1, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 9, 8, 7, 6, 5, 4, 3, 2, 1);   // vector of imag parts// vector of imag parts
        __m128i c16x8 = _mm_add_epi8(r16x8, j16x8);                                                   // c = r - i*j  

        print_m128i_16x8(&r16x8, "r16x8");
        print_m128i_16x8(&j16x8, "j16x8");
        print_m128i_16x8(&c16x8, "c16x8");

        __m128i vc8x8_a = _MM_SETR_EPI8( 8,  8,   7,   7,   6,   6,   5,   5,   4,   4,   3,   3,   2,   2, 1, 1); // vector of complex <tuple>(MSBimag, LSBreal)// vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc8x8_b = _MM_SETR_EPI8(-1, -1, 0xf, 0xf, 0xe, 0xe, 0xd, 0xd, 0xc, 0xc, 0xb, 0xb, 0xa, 0xa, 9, 9); // vector of complex <tuple>(MSBimag, LSBreal)// vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc8x8_c = _mm_add_epi8(vc8x8_a, vc8x8_b);                                                          // c = a + b

        print_m128i_16x8(&vc8x8_a, "vc8x8_a");
        print_m128i_16x8(&vc8x8_b, "vc8x8_b");
        print_m128i_16x8(&vc8x8_c, "vc8x8_c");
    }

    // Packet 8 16bits: representing 4 16b complex elements
    // (real16b, imag16b), (real16b, imag16b), (real16b, imag16b), (real16b, imag16b)
    {
        __m128i r8x16 = _MM_SETR_EPI16(6, 5, 4, 3, 2, 1, 0, -1); // vector of real parts
        __m128i j8x16 = _MM_SETR_EPI16(6, 5, 4, 3, 2, 1, 0, -1); // vector of imag parts
        __m128i c8x16 = _mm_add_epi16(r8x16, j8x16);             // c = r - i*j

        __m128i vc4x16_a = _MM_SETR_EPI16(2, 2, 1, 1, 0, 0, -1, -1); // vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc4x16_b = _MM_SETR_EPI16(6, 6, 5, 5, 4, 4,  3,  3); // vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc4x16_c = _mm_add_epi16(vc4x16_a, vc4x16_b);        // c = a + b


        print_m128i_8x16(&r8x16, "r8x16");
        print_m128i_8x16(&j8x16, "j8x16");
        print_m128i_8x16(&c8x16, "c8x16");
    }

    // Packetd 4 32bits: representing 2 32b complex elements
    // 2 32b complex (real32b, imag32b), (real32b, imag32b)
    {
        __m128i r4x32    = _MM_SETR_EPI32(1, 0, -1, -2); // vector of real parts
        __m128i j4x32    = _MM_SETR_EPI32(1, 0, -1, -2); // vector of imag parts
        __m128i c4x32    = _mm_add_epi32(r4x32, j4x32);  // c = r - i*j

        print_m128i_4x32(&r4x32, "r4x32");
        print_m128i_4x32(&j4x32, "j4x32");
        print_m128i_4x32(&c4x32, "c4x32");

        __m128i vc2x32_a = _MM_SETR_EPI32(-1, -1, -2, -2);    // vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc2x32_b = _MM_SETR_EPI32( 1,  1,  0,  0);    // vector of complex <tuple>(MSBimag, LSBreal)
        __m128i vc2x32_c = _mm_add_epi32(vc2x32_a, vc2x32_b); // c = a + b

        print_m128i_4x32(&vc2x32_a, "vc2x32_a");
        print_m128i_4x32(&vc2x32_b, "vc2x32_b");
        print_m128i_4x32(&vc2x32_c, "vc2x32_c");
    }
}

/*

Output on Console:
==================

r16x8: 0xff 0x0f 0x0e 0x0d 0x0c 0x0b 0x0a 0x09 0x08 0x07 0x06 0x05 0x04 0x03 0x02 0x01
j16x8: 0xff 0x0f 0x0e 0x0d 0x0c 0x0b 0x0a 0x09 0x08 0x07 0x06 0x05 0x04 0x03 0x02 0x01
c16x8: 0xfe 0x1e 0x1c 0x1a 0x18 0x16 0x14 0x12 0x10 0x0e 0x0c 0x0a 0x08 0x06 0x04 0x02
vc8x8_a: 0x08 0x08 0x07 0x07 0x06 0x06 0x05 0x05 0x04 0x04 0x03 0x03 0x02 0x02 0x01 0x01
vc8x8_b: 0xff 0xff 0x0f 0x0f 0x0e 0x0e 0x0d 0x0d 0x0c 0x0c 0x0b 0x0b 0x0a 0x0a 0x09 0x09
vc8x8_c: 0x07 0x07 0x16 0x16 0x14 0x14 0x12 0x12 0x10 0x10 0x0e 0x0e 0x0c 0x0c 0x0a 0x0a
r8x16: 0x0006 0x0005 0x0004 0x0003 0x0002 0x0001 0x0000 0xffff
j8x16: 0x0006 0x0005 0x0004 0x0003 0x0002 0x0001 0x0000 0xffff
c8x16: 0x000c 0x000a 0x0008 0x0006 0x0004 0x0002 0x0000 0xfffe
r4x32: 0x00000001 0x00000000 0xffffffff 0xfffffffe
j4x32: 0x00000001 0x00000000 0xffffffff 0xfffffffe
c4x32: 0x00000002 0x00000000 0xfffffffe 0xfffffffc
vc2x32_a: 0xffffffff 0xffffffff 0xfffffffe 0xfffffffe
vc2x32_b: 0x00000001 0x00000001 0x00000000 0x00000000
vc2x32_c: 0x00000000 0x00000000 0xfffffffe 0xfffffffe


*/
