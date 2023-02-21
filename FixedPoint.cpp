
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
    // Packed 16 8bits
    {
        __m128i a16x8 = _MM_SETR_EPI8(-1, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 9, 8, 7, 6, 5, 4, 3, 2, 1);
        __m128i b16x8 = _MM_SETR_EPI8(-1, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 9, 8, 7, 6, 5, 4, 3, 2, 1);
        __m128i c16x8 = _mm_add_epi8(a16x8, b16x8);

        print_m128i_16x8(&a16x8, "a16x8");
        print_m128i_16x8(&b16x8, "b16x8");
        print_m128i_16x8(&c16x8, "c16x8");
    }

    // Packet 8 16bits
    {
        __m128i a8x16 = _MM_SETR_EPI16(6, 5, 4, 3, 2, 1, 0, -1);
        __m128i b8x16 = _MM_SETR_EPI16(6, 5, 4, 3, 2, 1, 0, -1);
        __m128i c8x16 = _mm_add_epi16(a8x16, b8x16);

        print_m128i_8x16(&a8x16, "a8x16");
        print_m128i_8x16(&b8x16, "b8x16");
        print_m128i_8x16(&c8x16, "c8x16");
    }

    // Packetd 4 32bits
    {
        __m128i a4x32 = _MM_SETR_EPI32(1, 0, -1, -2);
        __m128i b4x32 = _MM_SETR_EPI32(1, 0, -1, -2);
        __m128i c4x32 = _mm_add_epi32(a4x32, b4x32);

        print_m128i_4x32(&a4x32, "a4x32");
        print_m128i_4x32(&b4x32, "b4x32");
        print_m128i_4x32(&c4x32, "c4x32");
    }
}


/*

Output on Console:

a16x8: 0xff 0x0f 0x0e 0x0d 0x0c 0x0b 0x0a 0x09 0x08 0x07 0x06 0x05 0x04 0x03 0x02 0x01
b16x8: 0xff 0x0f 0x0e 0x0d 0x0c 0x0b 0x0a 0x09 0x08 0x07 0x06 0x05 0x04 0x03 0x02 0x01
c16x8: 0xfe 0x1e 0x1c 0x1a 0x18 0x16 0x14 0x12 0x10 0x0e 0x0c 0x0a 0x08 0x06 0x04 0x02
a8x16: 0x0006 0x0005 0x0004 0x0003 0x0002 0x0001 0x0000 0xffff
b8x16: 0x0006 0x0005 0x0004 0x0003 0x0002 0x0001 0x0000 0xffff
c8x16: 0x000c 0x000a 0x0008 0x0006 0x0004 0x0002 0x0000 0xfffe
a4x32: 0x00000001 0x00000000 0xffffffff 0xfffffffe
b4x32: 0x00000001 0x00000000 0xffffffff 0xfffffffe
c4x32: 0x00000002 0x00000000 0xfffffffe 0xfffffffc





*/
