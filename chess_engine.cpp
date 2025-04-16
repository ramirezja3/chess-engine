#include <iostream>
#include <chrono>
#include "tables.h"
#include "position.h"
#include "types.h"
#include "pieces.h"


//Computes the perft of the position for a given depth, using bulk-counting
//According to the https://www.chessprogramming.org/Perft site:
//Perft is a debugging function to walk the move generation tree of strictly legal moves to count 
//all the leaf nodes of a certain depth, which can be compared to predetermined values and used to isolate bugs
template<Color Us>
unsigned long long perft(Position& p, unsigned int depth) {
	int nmoves;
	unsigned long long nodes = 0;

	MoveList<Us> list(p);

	if (depth == 1) return (unsigned long long) list.size();

	for (Move move : list) {
		p.play<Us>(move);
		nodes += perft<~Us>(p, depth - 1);
		p.undo<Us>(move);
	}

	return nodes;
}

//A variant of perft, listing all moves and for each move, the perft of the decremented depth
//It is used solely for debugging
template<Color Us>
void perftdiv(Position& p, unsigned int depth) {
	unsigned long long nodes = 0, pf;

	MoveList<Us> list(p);

	for (Move move : list) {
		std::cout << move;

		p.play<Us>(move);
		pf = perft<~Us>(p, depth - 1);
		std::cout << ": " << pf << " moves\n";
		nodes += pf;
		p.undo<Us>(move);
	}

	std::cout << "\nTotal: " << nodes << " moves\n";
}

void test_perft() {
	Position p;
	Position::set("rnbqkbnr/pppppppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq -", p);
	std::cout << p;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	auto n = perft<WHITE>(p, 6);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	auto diff = end - begin;

	std::cout << "Nodes: " << n << "\n";
	std::cout << "NPS: "
		<< int(n * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(diff).count())
		<< "\n";
	std::cout << "Time difference = "
		<< std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " [microseconds]\n";
}

template<Color Us>
Move find_best_move(Position& pos) {
    Move moves[256];
    Move* end = pos.generate_legals<Us>(moves);
    int best_score = -100000;
    Move best_move;

    for (Move* m = moves; m != end; ++m) {
        pos.play<Us>(*m);

        int score = evaluate(pos, Us);  // Pass side perspective

        if (score > best_score) {
            best_score = score;
            best_move = *m;
        }

        pos.undo<Us>(*m);
    }

    return best_move;
}


std::string getPos(std::string FEN) {
	std::cout<<"input FEN here"<<std::endl;
	std::cin>>FEN;
	
	return FEN;
}

int main() {
	//Make sure to initialise all databases before using the library!
	initialise_all_databases();
	zobrist::initialise_zobrist_keys();
	
	std::string FEN = DEFAULT_FEN;
	FEN = getPos(FEN);
	Position pos;
	Position::set(FEN, pos);
    test_perft();

	Move best;
	if(pos.turn() == WHITE) best = find_best_move<WHITE>(pos);
	else best = find_best_move<BLACK>(pos);

	std::cout << "Best move: " << best.from() << " -> " << best.to() << std::endl;
    std::cout << "Eval: " << pieces::getEval(pos, pos.turn()) << " (centipawns)\n";

	return 0;
}