#pragma once

#define INTENATIONAL_DRAUGHTS 1

#if (INTENATIONAL_DRAUGHTS == 1) // international draughts
    #define RUSSIAN_DAM_RULES 0
    #define BOARD_SIZE 10
    typedef unsigned long long bitmap_t;
    //#warning "majority rule is not implemented"
#else // russian draughts
    #define RUSSIAN_DAM_RULES 1
    #define BOARD_SIZE 8
    typedef unsigned bitmap_t;
#endif

#define BITMAP_SIZE ((BOARD_SIZE) * (BOARD_SIZE) / 2)


