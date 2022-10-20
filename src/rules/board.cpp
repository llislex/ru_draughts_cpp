#include "board.h"
#include "bit_op.h"

BoardStat::BoardStat(const BoardBin& b, const BoardGeometry& g, BoardBitmap marker)
    : b(b), g(g), marker(marker),
      own_units(bitcount(b.own)), own_dams(bitcount(b.own & b.dam)),
      enemy_units(bitcount(b.enemy)), enemy_dams(bitcount(b.enemy & b.dam))
{
}

int BoardStat::evaluate() const
{
    if (own_units == 0) return -127;
    if (enemy_units == 0) return 127;
    return own_units + 2 * own_dams - enemy_units - 2 * enemy_dams;
}

char BoardStat::unit_symbol(unsigned n) const
{
    const unsigned mask = 1 << n;
    if (mask & marker)
    {
        return '*';
    }
    return b.own & mask ? b.dam & mask ? 'O' : 'o' : b.enemy & mask ? b.dam & mask ? 'X' : 'x' : '.';
}

const int opposite_dir[BoardGeometry::Directions] = { BoardGeometry::RightUp, BoardGeometry::LeftUp, BoardGeometry::RightDown, BoardGeometry::LeftDown };

unsigned BoardGeometry::near(unsigned n, bool left, bool down) const
{
    if (n >= N)
        return N;
    const int c = col(n) + (left ? -1 : 1);
    if (outside(c))
        return N;
    const int r = row(n) + (down ? 1 : -1);
    if (outside(r))
        return N;
    return number(c, r);
}

unsigned BoardGeometry::near(unsigned n, Direction d) const
{
    return near(n, is_left(d), is_down(d));
}

unsigned BoardGeometry::near_at(unsigned n, Direction d, unsigned distance) const
{ 
    for (; distance; --distance)
    {
        n = near(n, is_left(d), is_down(d));
    }
    return n;
}

BoardBitmap BoardGeometry::near_mask(unsigned n, Direction d) const
{
    unsigned i = near(n, d);
    return i < N ? 1 << i : 0;
}

BoardBitmap BoardGeometry::neighbours_up(unsigned n) const
{
    return near_mask(n, LeftUp) | near_mask(n, RightUp);
}

BoardBitmap BoardGeometry::neighbours_down(unsigned n) const
{
    return near_mask(n, LeftDown) | near_mask(n, RightDown);
}

BoardBitmap BoardGeometry::neighbours(unsigned n) const
{
    return neighbours_up(n) | near_mask(n, LeftDown) | near_mask(n, RightDown);
}

BoardBitmap BoardGeometry::neighbours_at(unsigned n, unsigned distance) const
{
    BoardBitmap r = 0;
    for (int d = 0; d < Directions; ++d)
    {
        unsigned i = near_at(n, (Direction) d, distance);
        if (i < N)
            r |= 1 << i;
    }
    return r;
}

BoardBitmap BoardGeometry::neighbours_dir(unsigned n, Direction d) const
{
    BoardBitmap r = 0; 
    for (n = near(n, d); n != N; n = near(n, d)) 
    { 
        r |= 1 << n; 
    } 
    return r;
}
