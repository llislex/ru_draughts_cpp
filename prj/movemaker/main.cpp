#include <iostream>

#include "settings.h"
#include "rules.h"
#include "utils.h"

using namespace std;

int main(int argc, char** argv)
{
	cout << "movemaker" << endl;
	
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

	unsigned ply = 8;

	TookTime t;
	EvaluatedMoves moves;
	int eval = build_game_tree(b, r, white_turn, ply, moves);
	EvaluatedMoves bm = moves_with_eval(moves, eval);
	if (bm.size() == 0)
	{
		cout << (white_turn ? "Black win" : "White win") << endl;
		return 0;
	}
	srand(time(0));
	unsigned choise = rand() % bm.size();
	uint64_t t0 = t.took_time_ms();
	b = bm[choise].move.b;
	white_turn = !white_turn;
	BoardStat bs(b, r.bg);
	cout << bm[choise] <<  t0 << " ms" << " ply " << ply << endl;
	ply = ply_policy(bs.own_units + bs.enemy_units, bs.own_dams + bs.enemy_dams);
	
	return 0;
}
