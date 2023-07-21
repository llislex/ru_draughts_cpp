#include <iostream>
#include <algorithm>
#include "settings.h"
#include "rules.h"
#include "utils.h"


//using namespace std;

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
BoardBin b = brd(string(str));
BoardGeometry g(8);
Rules r(g);
bool white_turn = true;

int negamax_white(const BoardBin& b, unsigned depth, int alpha, int beta);
int negamax_black(const BoardBin& b, unsigned depth, int alpha, int beta);

int negamax_white(const BoardBin& b, unsigned depth, int alpha, int beta)
{
    if(depth == 0)
    {
        return extended_evaluate(b, r);
    }
    int result = MIN_EVAL;
    Rules::Moves moves;
    bool is_hit = r.move_list(b, moves);
    if (!is_hit)
        --depth;
    for(Rules::Moves::const_iterator m = moves.begin(); m != moves.end(); ++m)
    {
        int eval = negamax_black(m->b, depth, -beta, -alpha);
        result = std::max(result, -eval);
        alpha = std::max(result, alpha);
        if (alpha >= beta)
            break;
    }
    return result;
}

int negamax_black(const BoardBin& b, unsigned depth, int alpha, int beta)
{
    if(depth == 0)
    {
        return -extended_evaluate(b, r);
    }
    int result = MIN_EVAL;
    Rules::Moves moves;
    bool is_hit = r.move_list_enemy(b, moves);
    if (!is_hit)
        --depth;
    for(Rules::Moves::const_iterator m = moves.begin(); m != moves.end(); ++m)
    {
        int eval = negamax_white(m->b, depth, -beta, -alpha);
        result = std::max(result, -eval);
        alpha = std::max(result, alpha);
        if (alpha >= beta)
            break;
    }
    return result;
}


int pre_build_game_tree_white(const BoardBin& b, unsigned depth, EvaluatedMoves& eval_moves)
{
    int result = MIN_EVAL;
    int alpha = MIN_EVAL;
    int beta = MAX_EVAL;
    Rules::Moves moves;
    bool is_hit = r.move_list(b, moves);
    if (!is_hit)
        --depth;
    if(moves.size() == 1)
    {
        eval_moves.push_back(EvaluatedMove(*moves.begin(), 0));
        return 0;
    }
    for(Rules::Moves::const_iterator m = moves.begin(); m != moves.end(); ++m)
    {
        int eval = negamax_black(m->b, depth, -beta, -alpha);
        result = std::max(result, -eval);
        eval_moves.push_back(EvaluatedMove(*m, -eval));
    }
    return result;
}

int build_game_tree_white(const BoardBin& b, unsigned depth, EvaluatedMoves& eval_moves)
{
    int result = MIN_EVAL;
    int alpha = MIN_EVAL;
    int beta = MAX_EVAL;
    
	std::cout << "build_game_tree_white" << std::endl;
	EvaluatedMoves pre_moves;
	int pre_eval = pre_build_game_tree_white(b, depth / 2, pre_moves);
	if(pre_moves.size() == 1)
    {
        eval_moves.push_back(pre_moves[0]);
        return pre_eval;
    }
	std::sort(pre_moves.begin(), pre_moves.end(), [](const EvaluatedMove& a, const EvaluatedMove& b){return a.eval > b.eval;});

    for(auto m = pre_moves.begin(); m != pre_moves.end(); ++m)
    {
        int eval = negamax_black(m->move.b, depth, -beta, -alpha);
        result = std::max(result, -eval);
        eval_moves.push_back(EvaluatedMove(m->move, -eval));
    }
    return result;
}

int pre_build_game_tree_black(const BoardBin& b, unsigned depth, EvaluatedMoves& eval_moves)
{
    int result = MIN_EVAL;
    int alpha = MIN_EVAL;
    int beta = MAX_EVAL;
    Rules::Moves moves;
    bool is_hit = r.move_list_enemy(b, moves);
    if (!is_hit)
        --depth;
    if(moves.size() == 1)
    {
        eval_moves.push_back(EvaluatedMove(*moves.begin(), 0));
        return 0;
    }
    for(Rules::Moves::const_iterator m = moves.begin(); m != moves.end(); ++m)
    {
        int eval = negamax_white(m->b, depth, -beta, -alpha);
        result = std::max(result, -eval);
        eval_moves.push_back(EvaluatedMove(*m, -eval));
    }
    return result;
}

int build_game_tree_black(const BoardBin& b, unsigned depth, EvaluatedMoves& eval_moves)
{
    int result = MIN_EVAL;
    int alpha = MIN_EVAL;
    int beta = MAX_EVAL;
    Rules::Moves moves;
    bool is_hit = r.move_list_enemy(b, moves);
    if (!is_hit)
        --depth;
    if(moves.size() == 1)
    {
        eval_moves.push_back(EvaluatedMove(*moves.begin(), 0));
        return 0;
    }
    for(Rules::Moves::const_iterator m = moves.begin(); m != moves.end(); ++m)
    {
        int eval = negamax_white(m->b, depth, -beta, -alpha);
        result = std::max(result, -eval);
        eval_moves.push_back(EvaluatedMove(*m, -eval));
    }
    return result;
}

int main(int argc, char** argv)
{
	std::cout << "movemaker" << std::endl;
	unsigned ply = 12; 
	srand(time(0));

	TookTime t;
	EvaluatedMoves moves;
	int eval = white_turn ? build_game_tree_white(b, ply, moves) : build_game_tree_black(b, ply, moves);; 
	EvaluatedMoves bm = moves_with_eval(moves, eval);
	if (bm.size() == 0)
	{
		std::cout << (white_turn ? "Black win" : "White win") << std::endl;
		return 0;
	}
	unsigned choise = rand() % bm.size();
	uint64_t t0 = t.took_time_ms();
	b = bm[choise].move.b;
	white_turn = !white_turn;
	BoardStat bs(b, r.bg);
	std::cout << bm[choise] <<  t0 << " ms" << " ply " << ply << std::endl;
	ply = ply_policy(bs.own_units + bs.enemy_units, bs.own_dams + bs.enemy_dams);
	
	return 0;
}
