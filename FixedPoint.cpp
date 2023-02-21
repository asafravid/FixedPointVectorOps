
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

#define _MM_SETR_EPI8(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac, ad, ae, af) \
    {AS_CHAR(a0), AS_CHAR(a1), AS_CHAR(a2), AS_CHAR(a3), \
     AS_CHAR(a4), AS_CHAR(a5), AS_CHAR(a6), AS_CHAR(a7), \
     AS_CHAR(a8), AS_CHAR(a9), AS_CHAR(aa), AS_CHAR(ab), \
     AS_CHAR(ac), AS_CHAR(ad), AS_CHAR(ae), AS_CHAR(af)}

#define _MM_SETR_EPI16(a0, a1, a2, a3, a4, a5, a6, a7) \
    {AS_2CHARS(a0), AS_2CHARS(a1), AS_2CHARS(a2), AS_2CHARS(a3), \
     AS_2CHARS(a4), AS_2CHARS(a5), AS_2CHARS(a6), AS_2CHARS(a7)}

#define _MM_SETR_EPI32(a0, a1, a2, a3) \
    {AS_4CHARS(a0), AS_4CHARS(a1), AS_4CHARS(a2), AS_4CHARS(a3)}   

void print_m128i(__m128i* to_print, const char* name) {
    uint16_t val[8];
    memcpy(val, to_print, sizeof(val));
    printf("%s: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x \n", name, val[7], val[6], val[5], val[4], val[3], val[2], val[1], val[0]);
}

int main()
{
    // Packed 16 8bits
    {
        __m128i a16x8 = _MM_SETR_EPI8(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        __m128i b16x8 = _MM_SETR_EPI8(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        __m128i c16x8 = _mm_add_epi8(a16x8, b16x8);

        print_m128i(&a16x8, "a16x8");
        print_m128i(&b16x8, "b16x8");
        print_m128i(&c16x8, "c16x8");
    }

    // Packet 8 16bits
    {
        __m128i a8x16 = _MM_SETR_EPI16(1, 2, 3, 4, 5, 6, 7, 8);
        __m128i b8x16 = _MM_SETR_EPI16(1, 2, 3, 4, 5, 6, 7, 8);
        __m128i c8x16 = _mm_add_epi16(a8x16, b8x16);

        print_m128i(&a8x16, "a8x16");
        print_m128i(&b8x16, "b8x16");
        print_m128i(&c8x16, "c8x16");
    }

    // Packetd 4 32bits
    {
        __m128i a4x32 = _MM_SETR_EPI32(1, 2, 3, 4);
        __m128i b4x32 = _MM_SETR_EPI32(1, 2, 3, 4);
        __m128i c4x32 = _mm_add_epi32(a4x32, b4x32);

        print_m128i(&a4x32, "a4x32");
        print_m128i(&b4x32, "b4x32");
        print_m128i(&c4x32, "c4x32");
    }
}
