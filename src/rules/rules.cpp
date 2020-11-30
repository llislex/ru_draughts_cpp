#include "rules.h"
#include "..\utils\bit_op.h"
#include <assert.h>


Rules::Rules(const BoardGeometry& bg): bg(bg), dam_target(0),dam_target_enemy(0)
{
    for (unsigned i = 0; i < bg.N; ++i) //build ways map
    {
        way_up.push_back(bg.neighbours_up(i));
        way_down.push_back(bg.neighbours_down(i));
        ways.push_back(bg.neighbours(i));
        BoardBitmap all_ways_mask = 0;
        for (int d = 0; d < BoardGeometry::Directions; ++d)
        {
            BoardBitmap way_mask = bg.neighbours_dir(i, (BoardGeometry::Direction) d);
            dam_way_dir[d].push_back(way_mask);
            all_ways_mask |= way_mask;
        }
        dam_ways.push_back(all_ways_mask);
        ways2.push_back(bg.neighbours_at(i, 2));
    }
    for (int i = 0; i < bg.size / 2; ++i)
    {
        dam_target |= 1 << i;
        dam_target_enemy |= (1 << 31) >> i;
    }
}

Rules::~Rules()
{
}

bool Rules::_hit(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, Moves& moves, BoardBitmap taken) const
{
    bool result = false;
    const BoardBitmap to_hit = ways[n] & enemy & ~taken;
    if (to_hit)
    {
        const BoardBitmap to_jump = ways2[n] & ~(own | enemy);
        if (to_jump)
        {
            BoardBitmap hj = to_hit | to_jump;
            for (int d = 3; d >= 0; --d) //up direction check first
            {
                const BoardBitmap dir_mask = hj & dam_way_dir[d][n];
                if (bitcount(dir_mask) == 2) //TODO
                {
                    const BoardBitmap new_pos_mask = to_jump & dir_mask;
                    const BoardBitmap new_taken = taken | (to_hit & dir_mask);
                    unsigned new_n = lsb(new_pos_mask); //TODO
                    result = true;
#if (RUSSIAN_DAM_RULES == 1)
                    bool is_final = (dam_target & new_pos_mask) ? !_hit_dam(own, enemy, new_n, moves, new_taken, opposite_dir[d]) : !_hit(own, enemy, new_n, moves, new_taken);
#else
                    bool is_final = !_hit(own, enemy, new_n, moves, new_taken);
#endif
                    if (is_final)
                    {
                        Move m = make_move(own, enemy & ~new_taken, new_n, dam_target & new_pos_mask);
                        moves.push_back(m);
                    }
                    hj &= ~new_taken & ~new_pos_mask;
                    if (hj == 0)
                        break;
                }
            }
        }
    }
    return result;
}

bool Rules::_hit_dam(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, Moves& moves, BoardBitmap taken /*= 0*/, int back_dir /*= -1*/) const
{
    bool result = false;
    BoardBitmap all_ways = dam_ways[n];
    const BoardBitmap to_hit = all_ways & enemy & ~taken;
    if (to_hit)
    {
        const BoardBitmap to_jump = all_ways & ~(own | enemy);
        if (to_jump)
        {
            for (int d = 0; d < BoardGeometry::Directions; ++d)
            {
                if (d == back_dir)
                    continue;
                BoardBitmap dir_mask = dam_way_dir[d][n];
                const BoardBitmap to_hit_dir = to_hit & dir_mask;
                if (to_hit_dir)
                {
                    unsigned hit_target = bg.is_down(d) ? lsb(to_hit_dir) : msb(to_hit_dir);
                    const int new_back_dir = opposite_dir[d];
                    const BoardBitmap space = dam_way_dir[new_back_dir][hit_target] & dir_mask;
                    if ((space & (own | enemy)) == 0)
                    {
                        dir_mask = dam_way_dir[d][hit_target];
                        BoardBitmap to_jump_dir = dir_mask & to_jump;
                        const BoardBitmap obstacle_mask = to_jump_dir ^ dir_mask;
                        if (obstacle_mask)
                        {
                            unsigned obstacle_n = bg.is_down(d) ? lsb(obstacle_mask) : msb(obstacle_mask);
                            to_jump_dir &= dam_way_dir[new_back_dir][obstacle_n];
                        }
                        if (to_jump_dir)
                        {//for each bit
                            Moves final_hits;
                            bool all_final = true;
                            do
                            {
                                unsigned new_pos = take_msb(to_jump_dir);
                                BoardBitmap new_taken = taken | (1 << hit_target);
                                result = true;
                                all_final &= !_hit_dam(own, enemy, new_pos, moves, new_taken, new_back_dir);
                                if (all_final)
                                {
                                    Move m = make_move(own, enemy & ~new_taken, new_pos, true);
                                    m.b.dam = 1 << new_pos;
                                    final_hits.push_back(m);
                                }
                            } while (to_jump_dir);
                            if (all_final)
                            {
                                moves.insert(moves.begin(), final_hits.begin(), final_hits.end());
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

void Rules::_adjust_hit_list(Moves& hits, unsigned n0, BoardBitmap dam) const
{
    for(Moves::iterator it = hits.begin(); it != hits.end(); ++it)
    {
        it->b.dam = (it->b.dam | dam) & (it->b.own | it->b.enemy);
        it->n0 = n0;
    }
}

Rules::Move Rules::make_move(const BoardBin& bx, unsigned n0, unsigned n, bool dam) const
{
    Move m;
    m.n = n;
    m.n0 = n0;
    m.b.own = (bx.own & ~(1 << n0)) | (1 << n);
    m.b.enemy = bx.enemy;
    m.b.dam = bx.dam & ~(1 << n0);
    if (dam)
        m.b.dam |= 1 << n;
    return m;
}

Rules::Move Rules::make_move(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, bool dam) const
{
    Move m;
    m.n = n;
    m.b.own = own | (1 << n);
    m.b.enemy = enemy;
    m.b.dam = dam ? (1 << n) : 0;
    return m;
}

Rules::Move Rules::make_move_enemy(const BoardBin& bx, unsigned n0, unsigned n, bool dam) const
{
    Move m;
    m.n = n;
    m.n0 = n0;
    m.b.own = bx.own;
    m.b.enemy = (bx.enemy & ~(1 << n0)) | (1 << n);
    m.b.dam = bx.dam & ~(1 << n0);
    if (dam)
        m.b.dam |= 1 << n;
    return m;
}

Rules::Move Rules::make_move_enemy(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, bool dam) const
{
    Move m;
    m.n = n;
    m.b.own = own;
    m.b.enemy = enemy | (1 << n);
    m.b.dam = dam ? 1 << n : 0;
    return m;
}

bool Rules::_move(const BoardBin& b, unsigned n, Moves& moves) const
{
    BoardBitmap move_mask = way_up[n] & ~(b.own | b.enemy);
    bool result = move_mask != 0;
    while (move_mask)
    {
        unsigned n1 = take_lsb(move_mask);
        Move m = make_move(b, n, n1, (dam_target & (1 << n1)));
        moves.push_back(m);
    }
    return result;
}

//find dam target
bool Rules::_move_dam(const BoardBin& b, unsigned n, Moves& moves) const
{
    bool result = false;
    for (int d = 0; d < BoardGeometry::Directions; ++d)
    {
        BoardBitmap move_mask = dam_way_dir[d][n] & ~(b.own | b.enemy);
        BoardBitmap limits = dam_way_dir[d][n] & (b.own | b.enemy);
        if(limits)
        {
            unsigned dir_limit = bg.is_down(d) ? lsb(limits) : msb(limits);
            move_mask &= dam_way_dir[opposite_dir[d]][dir_limit];
        }
        while (move_mask)
        {
            unsigned n1 = take_msb(move_mask);
            Move m = make_move(b, n, n1, true);
            moves.push_back(m);
            result = true;
        }
    }
    return result;
}


bool Rules::move_list(const BoardBin& b, Moves& m) const
{
    BoardBitmap own = b.own;
    bool hit_happened = false;
    Moves moves;
    while (own)
    {
        Moves hits;
        unsigned n = take_msb(own);
        const BoardBitmap mask = 1 << n;
        const bool dam = (b.dam & mask) != 0;
        dam ?  _hit_dam(b.own & ~mask, b.enemy, n, hits) : _hit(b.own & ~mask, b.enemy, n, hits);
        if(hits.size() > 0)
        {
            hit_happened = true;
            _adjust_hit_list(hits, n, b.dam);
            m.insert(m.end(), hits.begin(), hits.end());
            continue;
        }
        if (!hit_happened)
        {
            dam ? _move_dam(b, n, moves) : _move(b, n, moves);
        }
    }
    if (!hit_happened)
    {
        m.insert(m.end(), moves.begin(), moves.end());
    }
    return hit_happened;
}

//TODO remove copy&paste for enemy
bool Rules::_hit_enemy(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, Moves& moves, BoardBitmap taken) const
{
    bool result = false;
    const BoardBitmap to_hit = ways[n] & own & ~taken;
    if (to_hit)
    {
        const BoardBitmap to_jump = ways2[n] & ~(own | enemy);
        if (to_jump)
        {
            BoardBitmap hj = to_hit | to_jump;
            for (int d = 3; d >= 0; --d) //up direction check first
            {
                const BoardBitmap dir_mask = hj & dam_way_dir[d][n];
                if (bitcount(dir_mask) == 2)
                {
                    const BoardBitmap new_pos_mask = to_jump & dir_mask;
                    const BoardBitmap new_taken = taken | (to_hit & dir_mask);
                    unsigned new_n = lsb(new_pos_mask);
                    result = true;
#if (RUSSIAN_DAM_RULES == 1)
                    bool is_final = (dam_target_enemy & new_pos_mask) ? !_hit_dam_enemy(own, enemy, new_n, moves, new_taken, opposite_dir[d]) : !_hit_enemy(own, enemy, new_n, moves, new_taken);
#else
                    bool is_final = !_hit_enemy(own, enemy, new_n, moves, new_taken);
#endif
                    if (is_final)
                    {
                        Move m = make_move_enemy(own & ~new_taken, enemy, new_n, dam_target_enemy & new_pos_mask);
                        moves.push_back(m);
                    }
                    hj &= ~new_taken & ~new_pos_mask;
                    if (hj == 0)
                        break;
                }
            }
        }
    }
    return result;
}

//TODO remove copy&paste for enemy
bool Rules::_hit_dam_enemy(const BoardBitmap& own, const BoardBitmap& enemy, unsigned n, Moves& moves, BoardBitmap taken /*= 0*/, int back_dir /*= -1*/) const
{
    bool result = false;
    BoardBitmap all_ways = dam_ways[n];
    const BoardBitmap to_hit = all_ways & own & ~taken;
    if (to_hit)
    {
        const BoardBitmap to_jump = all_ways & ~(own | enemy);
        if (to_jump)
        {
            for (int d = 0; d < BoardGeometry::Directions; ++d)
            {
                if (d == back_dir)
                    continue;
                BoardBitmap dir_mask = dam_way_dir[d][n];
                const BoardBitmap to_hit_dir = to_hit & dir_mask;
                if (to_hit_dir)
                {
                    unsigned hit_target = bg.is_down(d) ? lsb(to_hit_dir) : msb(to_hit_dir);
                    const int new_back_dir = opposite_dir[d];
                    const BoardBitmap space = dam_way_dir[new_back_dir][hit_target] & dir_mask;
                    if ((space & (own | enemy)) == 0)
                    {
                        dir_mask = dam_way_dir[d][hit_target];
                        BoardBitmap to_jump_dir = dir_mask & to_jump;
                        const BoardBitmap obstacle_mask = to_jump_dir ^ dir_mask;
                        if (obstacle_mask)
                        {
                            unsigned obstacle_n = bg.is_down(d)? lsb(obstacle_mask) : msb(obstacle_mask);
                            to_jump_dir &= dam_way_dir[new_back_dir][obstacle_n];
                        }
                        if (to_jump_dir)
                        {//for each bit
                            Moves final_hits;
                            bool all_final = true;
                            do
                            {
                                unsigned new_pos = take_msb(to_jump_dir);
                                BoardBitmap new_taken = taken | (1 << hit_target);
                                result = true;
                                all_final &= !_hit_dam_enemy(own, enemy, new_pos, moves, new_taken, new_back_dir);
                                if (all_final)
                                {
                                    Move m = make_move_enemy(own & ~new_taken, enemy, new_pos, true);
                                    m.b.dam = 1 << new_pos;
                                    final_hits.push_back(m);
                                }
                            } while (to_jump_dir);
                            if (all_final)
                            {
                                moves.insert(moves.begin(), final_hits.begin(), final_hits.end());
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

//TODO remove copy&paste for enemy
bool Rules::_move_enemy(const BoardBin& b, unsigned n, Moves& moves) const
{
    BoardBitmap move_mask = way_down[n] & ~(b.own | b.enemy);
    bool result = move_mask != 0;
    while (move_mask)
    {
        unsigned n1 = take_msb(move_mask);
        Move m = make_move_enemy(b, n, n1, (dam_target_enemy & (1 << n1)));
        moves.push_back(m);
    }
    return result;
}

//TODO remove copy&paste for enemy
bool Rules::_move_dam_enemy(const BoardBin& b, unsigned n, Moves& moves) const
{
    bool result = false;
    for (int d = 0; d < BoardGeometry::Directions; ++d)
    {
        BoardBitmap move_mask = dam_way_dir[d][n] & ~(b.own | b.enemy);
        BoardBitmap limits = dam_way_dir[d][n] & (b.own | b.enemy);
        if(limits)
        {
            unsigned dir_limit = bg.is_down(d) ? lsb(limits) : msb(limits);
            move_mask &= dam_way_dir[opposite_dir[d]][dir_limit];
        }
        while (move_mask)
        {
            unsigned n1 = take_msb(move_mask);
            Move m = make_move_enemy(b, n, n1, true);
            moves.push_back(m);
            result = true;
        }
    }
    return result;
}

//TODO remove copy&paste for enemy
bool Rules::move_list_enemy(const BoardBin& b, Moves& m) const
{
    BoardBitmap enemy = b.enemy;
    bool hit_happened = false;
    Moves moves;
    while (enemy)
    {
        Moves hits;
        unsigned n = take_msb(enemy);
        const BoardBitmap mask = 1 << n;
        const bool dam = (b.dam & mask) != 0;
        dam ?  _hit_dam_enemy(b.own, b.enemy & ~mask, n, hits) : _hit_enemy(b.own, b.enemy & ~mask, n, hits);
        if(hits.size() > 0)
        {
            hit_happened = true;
            _adjust_hit_list(hits, n, b.dam);
            m.insert(m.end(), hits.begin(), hits.end());
            continue;
        }
        if (!hit_happened)
        {
            dam ? _move_dam_enemy(b, n, moves) : _move_enemy(b, n, moves);
        }
    }
    if (!hit_happened)
    {
        m.insert(m.end(), moves.begin(), moves.end());
    }
    return hit_happened;
}
