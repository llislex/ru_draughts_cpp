#pragma once

#include "settings.h"


#if (BITMAP_SIZE <= 32)
    #define BIT_MASK(n) (1 << (n))
#elif (BITMAP_SIZE == 50)
    #define BIT_MASK(n) (1ULL << (n))
#else
    #error "Check BITMAP_SIZE"
#endif


unsigned lsb(bitmap_t bmp);
unsigned msb(bitmap_t bmp);
unsigned take_msb(bitmap_t& bmp);
unsigned take_lsb(bitmap_t& bmp);
unsigned bitcount(bitmap_t bmp);

