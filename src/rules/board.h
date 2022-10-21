#pragma once

typedef unsigned BoardBitmap;

struct BoardBin
{
    BoardBitmap own;   // white
    BoardBitmap enemy; // black
    BoardBitmap dam;
};

/*convert square board coordinate to the linear number of a black piece
4x4 example size = 4, n = 0 .. N - 1, N = 8
 . .  0 1
. .  2 3
 . .  4 5
. .  6 7
*/
struct BoardGeometry
{
    unsigned col_index(unsigned n) const { return n % (size / 2); }
    int row(unsigned n) const { return n / (size / 2); }
    int col(unsigned n) const { return col_index(n) * 2 + 1 - (row(n) % 2); }
    bool outside(int index) const { return index < 0 || index >= size; }
    bool is_left(int d) const { return d == LeftUp || d == LeftDown; }
public:
    BoardGeometry(unsigned size) : size(size), N(size * size / 2) {}
    ~BoardGeometry() {}
    const int size;
    const unsigned N;
    typedef enum { LeftDown, RightDown, LeftUp, RightUp, Directions } Direction;
    unsigned number(unsigned _col, unsigned _row) const { return _row * size / 2 + _col / 2; }
    unsigned near(unsigned n, bool left, bool down) const;
    unsigned near(unsigned n, Direction d) const;
    unsigned near_at(unsigned n, Direction d, unsigned distance) const;
    BoardBitmap near_mask(unsigned n, Direction d) const;
    BoardBitmap neighbours_up(unsigned n) const;
    BoardBitmap neighbours_down(unsigned n) const;
    BoardBitmap neighbours(unsigned n) const;
    BoardBitmap neighbours_at(unsigned n, unsigned distance) const;
    BoardBitmap neighbours_dir(unsigned n, Direction d) const;
    bool is_down(int d) const { return d == RightDown || d == LeftDown; }
    unsigned mirror(unsigned n) const {return N - n;}
};

extern const int opposite_dir[BoardGeometry::Directions];

struct BoardStat
{
    BoardStat(const BoardBin& b, const BoardGeometry& g, BoardBitmap marker = 0);
    const BoardBin& b;
    const BoardGeometry& g;
    const BoardBitmap marker;
    char unit_symbol(unsigned n) const;
    const int own_units;
    const int own_dams;
    const int enemy_units;
    const int enemy_dams;
};

