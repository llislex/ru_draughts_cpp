#pragma once
#include "board.h"
#include <vector>
#include <list>

/*struct Move
{
    Move() : taken(0), visited(0) {}
    BoardBitmap s;
    BoardBitmap e;
    BoardBitmap taken;
    BoardBitmap visited;
};*/

class Rules
{
public:
    typedef std::vector<BoardBin> Moves;

public:
    static std::vector<BoardBitmap> way_up; //distance 1
    static std::vector<BoardBitmap> way_down; //distance 1
    static std::vector<BoardBitmap> ways; //distance 1
    static std::vector<BoardBitmap> ways2; //distance 2
    static std::vector<BoardBitmap> dam_ways;
    static std::vector<BoardBitmap> dam_way_dir[BoardGeometry::Directions];
    static BoardGeometry bg;
    static BoardBitmap dam_target;

    static void _adjust_hit_list(Moves& hits, unsigned n0, BoardBitmap dam);

    static unsigned _hit(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, Moves& moves, BoardBitmap taken = 0); //return number of taken units
    static unsigned _hit_dam(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, Moves& moves, BoardBitmap taken = 0, int back_dir = -1); //return number of taken units
    static bool _move(const BoardBin& b, unsigned n, Moves& moves);
    static bool _move_dam(const BoardBin& b, unsigned n, Moves& moves);

public:
    static void init();
    static BoardBin mirror(const BoardBin& b);
    static BoardBin control_area(const BoardBin& b);

    static bool is_hit(const BoardBin& b);
    static bool move_list(const BoardBin& b, Moves& moves);
    static bool hit_list(const BoardBin& b, Moves& moves);
};
