#include <iostream>
#include <algorithm>
#include "settings.h"
#include "rules.h"
#include "utils.h"


//using namespace std;
#if 0
const  char*  str =
	"  x  x  x  x"
	"x  x  x  x  "
	"  x  x  x  x"
	".  .  .  .  "
	"  .  .  .  ."
	"o  o  o  o  "
	"  o  o  o  o"
	"o  o  o  o  "
	;
#else
const  char*  str =
    "  x  x  x  ."
    "x  x  x  x  "
    "  x  x  .  x"
    "x  .  x  .  "
    "  .  .  .  ."
    "o  o  o  o  "
    "  o  .  o  ."
    "o  o  o  o  "
    ;
#endif
typedef struct
{
    int n;
    int ab;
    int ep;
} stat_t;

BoardBin b = brd(string(str));
BoardGeometry g(8);
Rules r(g);
bool white_turn = true;
stat_t g_stat = {0};

double percent_ratio(int a, int b)
{
    return ((double) a) * 100 / ((double) b);
}

void reset_stat()
{
    g_stat.n = 0;
    g_stat.ab = 0;
    g_stat.ep = 0;
}

std::ostream& operator<<(std::ostream& os, const stat_t& st)
{
    os << "n: " << st.n << " ep:" << st.ep << " ab skip: " << st.ab << std::endl;
    double ab_ratio = percent_ratio(st.ab, st.n);
    double ep_ratio = percent_ratio(st.ep, st.n);
    os << "ab skipped: " << ab_ratio << " %" << std::endl;
    os << "ep ratio:   " << ep_ratio << " %" << std::endl;
    return os;
}

int alphabeta_white(const BoardBin& b, unsigned depth, int alpha, int beta);
int alphabeta_black(const BoardBin& b, unsigned depth, int alpha, int beta);

int alphabeta_white(const BoardBin& b, unsigned depth, int alpha, int beta)
{
    if(depth == 0)
    {
        ++g_stat.ep;
        return extended_evaluate(b, r);
    }
    int result = MIN_EVAL;
    Rules::Moves moves;
    bool is_hit = r.move_list(b, moves);
    if (!is_hit)
        --depth;
    for(Rules::Moves::const_iterator m = moves.begin(); m != moves.end(); ++m)
    {
        ++g_stat.n;
        int eval = alphabeta_black(m->b, depth, alpha, beta);
        result = std::max(result, eval);
        if (result > beta)
        {
            ++g_stat.ab;
            break;
        }
        alpha = std::max(alpha, result);
    }
    return result;
}

int alphabeta_black(const BoardBin& b, unsigned depth, int alpha, int beta)
{
    if(depth == 0)
    {
        ++g_stat.ep;
        return extended_evaluate(b, r);
    }
    int result = MAX_EVAL;
    Rules::Moves moves;
    bool is_hit = r.move_list_enemy(b, moves);
    if(!is_hit)
        --depth;
    for(Rules::Moves::const_iterator m = moves.begin(); m != moves.end(); ++m)
    {
        ++g_stat.n;
        int eval = alphabeta_white(m->b, depth, alpha, beta);
        result = std::min(result, eval);
        if (result < alpha)
        {
            ++g_stat.ab;
            break;
        }
        beta = std::min(beta, result);
    }
    return result;
}

EvaluatedMoves pre_build_game_tree(const BoardBin& b, bool white, unsigned depth)
{
    int alpha = MIN_EVAL;
    int beta = MAX_EVAL;
    Rules::Moves moves;
    EvaluatedMoves eval_moves;
    white ? r.move_list(b, moves) : r.move_list_enemy(b, moves);
    if(moves.size() == 1)
    {
        eval_moves.push_back(EvaluatedMove(*moves.begin(), 0));
        return eval_moves;
    }
    for(Rules::Moves::const_iterator m = moves.begin(); m != moves.end(); ++m)
    {
        int eval = white ? alphabeta_black(m->b, depth, alpha, beta) : alphabeta_white(m->b, depth, alpha, beta);
        eval_moves.push_back(EvaluatedMove(*m, eval));
    }
    return eval_moves;
}


EvaluatedMoves build_game_tree_ex(const BoardBin& b, bool white, unsigned depth)
{
    int alpha = MIN_EVAL;
    int beta = MAX_EVAL;

    EvaluatedMoves eval_moves = pre_build_game_tree(b, white, depth / 2);
    if(eval_moves.size() == 1)
    {
        return eval_moves;
    }
    std::sort(eval_moves.begin(), eval_moves.end(), [white](auto& A, auto& B){
        return white ? A.eval > B.eval : A.eval < B.eval;
    });

    EvaluatedMoves final_moves;
    for(auto& mx: eval_moves)
    {
        if (white)
        {
            mx.eval = alphabeta_black(mx.move.b, depth, alpha, beta);
            alpha = std::max(alpha, mx.eval);
            if (alpha > beta)
            {
                ++g_stat.ab;
                break;
            }
            final_moves.push_back(mx);
        }
        else
        {
            mx.eval = alphabeta_white(mx.move.b, depth, alpha, beta);
            beta = std::min(beta, mx.eval);
            if (beta < alpha)
            {
                ++g_stat.ab;
                break;
            }
            final_moves.push_back(mx);
        }
    }
    return final_moves;
}

int main(int argc, char** argv)
{
	std::cout << "movemaker" << std::endl;
    unsigned ply = 8;
	srand(time(0));


    for(int i = 0; i < 5; ++i)
    {
        BoardBin next;
        {
            reset_stat();
            TookTime t;
            EvaluatedMoves moves = build_game_tree_ex(b, white_turn, ply);
            if (moves.size() == 0)
            {
                std::cout << (white_turn ? "Black win" : "White win") << std::endl;
                return 0;
            }
            std::sort(moves.begin(), moves.end(), [](auto& A, auto& B){
                return white_turn ? A.eval > B.eval : A.eval < B.eval;
            });

            //unsigned choise = rand() % bm.size();
            uint64_t t0 = t.took_time_ms();
            BoardStat bs(b, r.bg);
            std::cout << moves[0] <<  t0 << " ms" << " ply " << ply << std::endl;
            next = moves[0].move.b;
            std::cout << g_stat << std::endl;
        }
        std::cout << "-------------------" << std::endl;
        {
            reset_stat();
            TookTime t;
            EvaluatedMoves moves = pre_build_game_tree(b, white_turn, ply);
            if (moves.size() == 0)
            {
                std::cout << (white_turn ? "Black win" : "White win") << std::endl;
                return 0;
            }
            std::sort(moves.begin(), moves.end(), [](auto& A, auto& B){
                return white_turn ? A.eval > B.eval : A.eval < B.eval;
            });

            //unsigned choise = rand() % bm.size();
            uint64_t t0 = t.took_time_ms();
            BoardStat bs(b, r.bg);
            std::cout << moves[0] <<  t0 << " ms" << " ply " << ply << std::endl;
            std::cout << g_stat << std::endl;
        }
        std::cout << "----------------------------------------------------------" << std::endl;
        b = next;
        white_turn = !white_turn;

        //ply = ply_policy(bs.own_units + bs.enemy_units, bs.own_dams + bs.enemy_dams);
    }
	return 0;
}
