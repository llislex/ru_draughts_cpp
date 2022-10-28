#include "bit_op.h"

#if 0 //HARDWARE
#include <intrin.h>
unsigned msb(unsigned bmp)
{
    unsigned long position;
    _BitScanReverse(&position, bmp);
    return position;
}

unsigned lsb(unsigned bmp)
{
    unsigned long position;
    _BitScanForward(&position, bmp);
    return position;
}

unsigned bitcount(unsigned bmp)
{
    return __popcnt(bmp);
}
#else //NO HARDWARE
#define BITN(bmp, n) if(bmp & BIT_MASK(n)) return n
unsigned lsb(bitmap_t bmp)
{
    BITN(bmp, 0);
    BITN(bmp, 1);
    BITN(bmp, 2);
    BITN(bmp, 3);
    BITN(bmp, 4);
    BITN(bmp, 5);
    BITN(bmp, 6);
    BITN(bmp, 7);
    BITN(bmp, 8);
    BITN(bmp, 9);
    BITN(bmp, 10);
    BITN(bmp, 11);
    BITN(bmp, 12);
    BITN(bmp, 13);
    BITN(bmp, 14);
    BITN(bmp, 15);
    BITN(bmp, 16);
    BITN(bmp, 17);
    BITN(bmp, 18);
    BITN(bmp, 19);
    BITN(bmp, 20);
    BITN(bmp, 21);
    BITN(bmp, 22);
    BITN(bmp, 23);
    BITN(bmp, 24);
    BITN(bmp, 25);
    BITN(bmp, 26);
    BITN(bmp, 27);
    BITN(bmp, 28);
    BITN(bmp, 29);
    BITN(bmp, 30);
    BITN(bmp, 31);
#if (BITMAP_SIZE == 50)
    BITN(bmp, 32);
    BITN(bmp, 33);
    BITN(bmp, 34);
    BITN(bmp, 35);
    BITN(bmp, 36);
    BITN(bmp, 37);
    BITN(bmp, 38);
    BITN(bmp, 39);
    BITN(bmp, 40);
    BITN(bmp, 41);
    BITN(bmp, 42);
    BITN(bmp, 43);
    BITN(bmp, 44);
    BITN(bmp, 45);
    BITN(bmp, 46);
    BITN(bmp, 47);
    BITN(bmp, 48);
    BITN(bmp, 49);
    BITN(bmp, 50);
#endif //BITMAP_SIZE == 50
    return BITMAP_SIZE;
}

unsigned msb(bitmap_t bmp)
{
#if (BITMAP_SIZE == 50)
    BITN(bmp, 50);
    BITN(bmp, 49);
    BITN(bmp, 48);
    BITN(bmp, 47);
    BITN(bmp, 46);
    BITN(bmp, 45);
    BITN(bmp, 44);
    BITN(bmp, 43);
    BITN(bmp, 42);
    BITN(bmp, 41);
    BITN(bmp, 40);
    BITN(bmp, 39);
    BITN(bmp, 38);
    BITN(bmp, 37);
    BITN(bmp, 36);
    BITN(bmp, 35);
    BITN(bmp, 34);
    BITN(bmp, 33);
    BITN(bmp, 32);
#endif //BITMAP_SIZE == 50
    BITN(bmp, 31);
    BITN(bmp, 30);
    BITN(bmp, 29);
    BITN(bmp, 28);
    BITN(bmp, 27);
    BITN(bmp, 26);
    BITN(bmp, 25);
    BITN(bmp, 24);
    BITN(bmp, 23);
    BITN(bmp, 22);
    BITN(bmp, 21);
    BITN(bmp, 20);
    BITN(bmp, 19);
    BITN(bmp, 18);
    BITN(bmp, 17);
    BITN(bmp, 16);
    BITN(bmp, 15);
    BITN(bmp, 14);
    BITN(bmp, 13);
    BITN(bmp, 12);
    BITN(bmp, 11);
    BITN(bmp, 10);
    BITN(bmp, 9);
    BITN(bmp, 8);
    BITN(bmp, 7);
    BITN(bmp, 6);
    BITN(bmp, 5);
    BITN(bmp, 4);
    BITN(bmp, 3);
    BITN(bmp, 2);
    BITN(bmp, 1);
    BITN(bmp, 0);
    return BITMAP_SIZE;
}

const unsigned bbc_lut[256] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};
unsigned bitcount(bitmap_t i)
{
    const unsigned char* const b = (unsigned char*)&i;
    return  bbc_lut[*b] + bbc_lut[*(b+1)] + bbc_lut[*(b+2)] + bbc_lut[*(b+3)]
#if (BITMAP_SIZE == 50)
            + bbc_lut[*(b+4)]+ bbc_lut[*(b+5)]+bbc_lut[*(b+6)]
#endif //BITMAP_SIZE == 50
    ;
}

unsigned take_msb(bitmap_t& bmp)
{
    unsigned n = msb(bmp);
    bmp &= ~BIT_MASK(n);
    return n;
}

unsigned take_lsb(bitmap_t& bmp)
{
    unsigned n = lsb(bmp);
    bmp &= ~BIT_MASK(n);
    return n;
}
#endif //INTRINCIC
