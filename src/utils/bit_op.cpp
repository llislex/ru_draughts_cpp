#include "bit_op.h"
#if 0 //NO INTRINSIC
#define BITN(bmp, n) if(bmp & (1 << n)) return n
unsigned lsb(unsigned bmp)
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
    return 32;
}

unsigned msb(unsigned bmp)
{
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
    return 32;
}

unsigned bitcount(unsigned i)
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}
#else
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
#endif //INTRINCIC
unsigned take_msb(unsigned& bmp)
{
    unsigned n = msb(bmp);
    bmp &= ~(1 << n);
    return n;
}

unsigned take_lsb(unsigned& bmp)
{
    unsigned n = lsb(bmp);
    bmp &= ~(1 << n);
    return n;
}
