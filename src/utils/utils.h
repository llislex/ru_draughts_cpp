#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "..\rules\rules.h"
#include "..\ai\ai.h"
#include <time.h>
#include <chrono>

using namespace std;

inline uint64_t timestamp()
{
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

struct TookTime
{
    uint64_t ts;
    TookTime(): ts(timestamp()){}
    uint64_t took_time_ms() const { return timestamp() - ts;};
};

//return errno
inline int board_error(const BoardBin& b, const BoardGeometry& g)
{
    const unsigned occupied = b.own | b.enemy;
    if ((b.own ^ b.enemy) != occupied)
        return -1;
    if ((occupied & b.dam) != b.dam)
        return -2;
    const unsigned bits_in_bitmap = sizeof(BoardBitmap) * 8;
    if (bits_in_bitmap < g.N)
        return -3;
    if (bits_in_bitmap > g.N)
        if ((((1 << g.N) - 1) & occupied) != occupied)
            return -4;
    return 0;
}

inline ostream& operator<<(ostream& os, const BoardStat& bs)
{
    const int o = bs.own_units - bs.own_dams;
    const int O = bs.own_dams;
    const int x = bs.enemy_units - bs.enemy_dams;
    const int X = bs.enemy_dams;
    os << "o:" << o << " O:" << O << " x:" << x << " X:" << X << "  err:" << board_error(bs.b, bs.g) << endl;
    for (int r = 0; r < bs.g.size; ++r)
    {
        os << endl;
        for (int c = 0; c < bs.g.size; ++c)
        {
            if (((r + c) % 2) == 1)
            {
                unsigned n = bs.g.number(c, r);
                os << bs.unit_symbol(n);
            }
            else
                os << " ";
        }
    }
    return os;
}

inline ostream& operator<<(ostream& os, const Rules::Move& m)
{
    BoardGeometry bg(8);
    BoardStat bs(m.b, bg, 1 << m.n0);
    os << bs << endl << m.n0 << "->" << m.n << endl;
    os << endl;
    return os;
}

inline ostream& operator<<(ostream& os, const Rules::Moves& it)
{
    int num = 1;
    for (Rules::Moves::const_iterator i = it.begin(); i != it.end(); ++i)
    {
        os << num << endl;
        os << *i << endl;
        ++num;
    }
    return os;
}

inline BoardBin brd(const string& st)
{
    BoardBin b = { 0, 0, 0 };
    BoardBitmap mask = 1;
    for (string::const_iterator it = st.begin(); it != st.end(); ++it)
    {
        switch (*it)
        {
            case 'o': 
                b.own |= mask;
                mask <<= 1;
                break;
            case 'O':
                b.own |= mask;
                b.dam |= mask;
                mask <<= 1;
                break;
            case 'X':
                b.enemy |= mask;
                b.dam |= mask;
                mask <<= 1;
                break;
            case 'x':
                b.enemy |= mask;
                mask <<= 1;
                break;
            case '.':
                mask <<= 1;
                break;
        }
    }
    return b;
}

inline void show_bitmap(BoardBitmap bmp)
{
    BoardBin b = { 0, 0, 0 };
    b.enemy = bmp;
    BoardGeometry bg(8);
    BoardStat bs(b, bg);
    cout << bs << endl;
}

inline ostream& operator<<(ostream& os, const vector<int>& it)
{
    os << it.size() << " : ";
    for (vector<int>::const_iterator i = it.begin(); i != it.end(); ++i)
    {
        os << *i << " ";
    }
    return os;
}

inline void indent(ostream& os, int level)
{
    while(level--)
        os << " ";
}

inline ostream& operator<<(ostream& os, const EvaluatedMove& m)
{
    return os << m.move << "eval " << m.eval << endl;
}

inline ostream& operator<<(ostream& os, const EvaluatedMoves& moves)
{
    for(EvaluatedMoves::const_iterator it = moves.begin(); it != moves.end(); ++it)
    {
        os << *it << endl;
    }
    return os;
}

inline bool operator==(const BoardBin& a, const BoardBin& b)
{
    return a.dam == b.dam && a.own == b.own && a.enemy && b.enemy;
}
