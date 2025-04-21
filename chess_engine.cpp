#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "position.h"
#include "tables.h"
#include "types.h"
#include "pieces.h" // Assume evaluate() and piece_value() come from here

std::vector<std::string> move_history;

std::string move_to_algebraic(const Move& m, const Position& pos) {
    Piece moving_piece = pos.at(m.from());
    std::string notation;

    if (m.flags() == OO) return "O-O";
    if (m.flags() == OOO) return "O-O-O";

    PieceType pt = type_of(moving_piece);
    if (pt != PAWN) notation += PIECE_STR[moving_piece];

    if (pos.at(m.to()) != NO_PIECE || m.flags() == EN_PASSANT) {
        if (pt == PAWN) notation += SQSTR[m.from()][0];
        notation += "x";
    }

    notation += SQSTR[m.to()];

    switch (m.flags()) {
        case PR_QUEEN: case PC_QUEEN: notation += "=Q"; break;
        case PR_ROOK:  case PC_ROOK:  notation += "=R"; break;
        case PR_BISHOP:case PC_BISHOP:notation += "=B"; break;
        case PR_KNIGHT:case PC_KNIGHT:notation += "=N"; break;
        default: break;
    }

    Position tmp = pos;
    if (pos.turn() == WHITE)
        tmp.play<WHITE>(m);
    else
        tmp.play<BLACK>(m);

    Move dummy[256];
    if ((tmp.turn() == WHITE && tmp.in_check<WHITE>()) ||
        (tmp.turn() == BLACK && tmp.in_check<BLACK>())) {
        auto* end = tmp.turn() == WHITE ? tmp.generate_legals<WHITE>(dummy) : tmp.generate_legals<BLACK>(dummy);
        notation += (end == dummy) ? "#" : "+";
    }

    return notation;
}

Move parse_uci(const std::string& str) {
    if (str.length() < 4) return Move();
    return Move(str);
}

template<Color C>
bool is_checkmate_or_stalemate(Position& pos) {
    Move moves[256];
    Move* end = pos.generate_legals<C>(moves);

    if (end == moves) {
        if (pos.in_check<C>()) {
            std::cout << "Checkmate! ";
            std::cout << (C == WHITE ? "Black" : "White") << " wins.\n";
        } else {
            std::cout << "Stalemate! Draw.\n";
        }
        return true;
    }
    return false;
}

bool is_insufficient_material(const Position& pos) {
    int white_material = 0;
    int black_material = 0;

    for (int pt = PAWN; pt < KING; ++pt) {
        int w = pop_count(pos.bitboard_of(WHITE, PieceType(pt)));
        int b = pop_count(pos.bitboard_of(BLACK, PieceType(pt)));
        white_material += w * piece_value(PieceType(pt));
        black_material += b * piece_value(PieceType(pt));
    }

    if (white_material == 0 && black_material == 0)
        return true;

    int total_white = pop_count(pos.bitboard_of(WHITE, KNIGHT)) +
                      pop_count(pos.bitboard_of(WHITE, BISHOP));
    int total_black = pop_count(pos.bitboard_of(BLACK, KNIGHT)) +
                      pop_count(pos.bitboard_of(BLACK, BISHOP));

    bool white_ok = white_material == 300 && total_white == 1;
    bool black_ok = black_material == 300 && total_black == 1;

    return (white_ok && black_material == 0) || (black_ok && white_material == 0);
}

template<Color Us>
Move find_best_move(Position& pos) {
    Move moves[256];
    Move* end = pos.generate_legals<Us>(moves);
    int best_score = -1000000;
    Move best;

    for (Move* m = moves; m != end; ++m) {
        pos.play<Us>(*m);
        int score = evaluate(pos, Us);
        pos.undo<Us>(*m);

        if (score > best_score) {
            best_score = score;
            best = *m;
        }
    }
    return best;
}

int main() {
    initialise_all_databases();
    zobrist::initialise_zobrist_keys();

    Position pos;
    Position::set(DEFAULT_FEN, pos);
    std::cout << pos;

    Color human_side;
    std::string choice;
    std::cout << "Play as (w/b)? ";
    std::cin >> choice;
    human_side = (choice == "w" || choice == "W") ? WHITE : BLACK;

    while (true) {
        std::cout << pos;

        if (is_insufficient_material(pos)) {
            std::cout << "Draw by insufficient material.\n";
            break;
        }

        bool game_over = pos.turn() == WHITE ?
            is_checkmate_or_stalemate<WHITE>(pos) :
            is_checkmate_or_stalemate<BLACK>(pos);

        if (game_over) break;

        if (pos.turn() == human_side) {
            std::string input;
            std::cout << "Your move (e.g. e2e4): ";
            std::cin >> input;

            Move move = parse_uci(input);
            Move moves[256];
            Move* end = human_side == WHITE ?
                pos.generate_legals<WHITE>(moves) :
                pos.generate_legals<BLACK>(moves);

            bool legal = false;
            for (Move* m = moves; m != end; ++m) {
                if (move.to_from() == m->to_from()) {
                    move = *m;
                    legal = true;
                    break;
                }
            }

            if (!legal) {
                std::cout << "Illegal move.\n";
                continue;
            }

            if (human_side == WHITE) pos.play<WHITE>(move);
            else pos.play<BLACK>(move);

            move_history.push_back(move_to_algebraic(move, pos));
        } else {
            std::cout << "Engine is thinking...\n";
            auto start = std::chrono::steady_clock::now();

            Move best = pos.turn() == WHITE ?
                find_best_move<WHITE>(pos) :
                find_best_move<BLACK>(pos);

            auto end = std::chrono::steady_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            std::cout << "Engine plays: " << SQSTR[best.from()] << SQSTR[best.to()] << " (" << ms << " ms)\n";

            if (pos.turn() == WHITE) pos.play<WHITE>(best);
            else pos.play<BLACK>(best);

            move_history.push_back(move_to_algebraic(best, pos));
        }

        std::cout << "\nMove History:\n";
        for (size_t i = 0; i < move_history.size(); ++i) {
            if (i % 2 == 0) std::cout << (i / 2 + 1) << ". ";
            std::cout << move_history[i] << " ";
            if (i % 2 == 1) std::cout << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}
