#pragma once
#include "board.h"
#include <vector>
#include <list>

struct Move
{
    Move() : taken(0), visited(0) {}
    BoardBitmap s;
    BoardBitmap e;
    BoardBitmap taken;
    BoardBitmap visited;
};

class Rules
{
public: //TODO make protected
    std::vector<BoardBitmap> way_up; //distance 1
    std::vector<BoardBitmap> way_down; //distance 1
    std::vector<BoardBitmap> ways; //distance 1
    std::vector<BoardBitmap> ways2; //distance 2
    std::vector<BoardBitmap> dam_ways;
    std::vector<BoardBitmap> dam_way_dir[BoardGeometry::Directions];
    const BoardGeometry& bg;
    BoardBitmap dam_target;
    BoardBitmap dam_target_enemy;

    struct Move
    {
        BoardBin b;  //state after move
        unsigned n0; //from
        unsigned n;  //to
    };
    typedef std::vector<Move> Moves;
    Move make_move(const BoardBin& bx, unsigned n0, unsigned n, bool dam = false) const;
    Move make_move(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, bool dam = false) const;
    Move make_move_enemy(const BoardBin& bx, unsigned n0, unsigned n, bool dam = false) const;
    Move make_move_enemy(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, bool dam = false) const;

    void _adjust_hit_list(Moves& hits, unsigned n0, BoardBitmap dam) const;

    unsigned _hit(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, Moves& moves, BoardBitmap taken = 0) const; //return number of taken units
    unsigned _hit_dam(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, Moves& moves, BoardBitmap taken = 0, int back_dir = -1) const; //return number of taken units
    bool _move(const BoardBin& b, unsigned n, Moves& moves) const;
    bool _move_dam(const BoardBin& b, unsigned n, Moves& moves) const;
    
    unsigned _hit_enemy(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, Moves& moves, BoardBitmap taken = 0) const;
    unsigned _hit_dam_enemy(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, Moves& moves, BoardBitmap taken = 0, int back_dir = -1) const;
    bool _move_enemy(const BoardBin& b, unsigned n, Moves& moves) const;
    bool _move_dam_enemy(const BoardBin& b, unsigned n, Moves& moves) const;
    
public:
    BoardBin _control_area(const BoardBin& b) const;

	bool move_list(const BoardBin& b, Moves& moves) const;
	bool move_list_enemy(const BoardBin& b, Moves& moves) const;
    BoardBin apply_move(const BoardBin& b, bool own, unsigned n0, unsigned n1) const;

	Rules(const BoardGeometry& bg);
	~Rules();
};
