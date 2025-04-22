#include "pieces.h"
#include "types.h"
#include "position.h"
#include <array>
#include <algorithm>

// --- Piece Values ---
int piece_value(PieceType pt) {
    switch (pt) {
        case PAWN:   return 100;
        case KNIGHT: return 320;
        case BISHOP: return 330;
        case ROOK:   return 500;
        case QUEEN:  return 900;
        case KING:   return 10000;
        default:     return 0;
    }
}

// --- Piece Square Tables ---

static const int PST_MG[6][64] = {
    // PAWN
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        5,10,10,-20,-20,10,10, 5,
        5,-5,-10, 0, 0,-10,-5, 5,
        0, 0, 0, 20, 20, 0, 0, 0,
        5, 5,10, 25,25,10, 5, 5,
       10,10,20, 30,30,20,10,10,
       50,50,50, 50,50,50,50,50,
        0, 0, 0,  0, 0, 0, 0, 0
    },
    // KNIGHT
    {
       -50,-40,-30,-30,-30,-30,-40,-50,
       -40,-20,  0,  0,  0,  0,-20,-40,
       -30,  0, 10, 15,15,10,  0,-30,
       -30,  5, 15,20,20,15,  5,-30,
       -30,  0, 15,20,20,15,  0,-30,
       -30,  5, 10,15,15,10,  5,-30,
       -40,-20,  0,  5, 5, 0,-20,-40,
       -50,-40,-30,-30,-30,-30,-40,-50
    },
    // BISHOP
    {
       -20,-10,-10,-10,-10,-10,-10,-20,
       -10,  0,  0,  0,  0,  0,  0,-10,
       -10,  0,  5, 10,10,  5,  0,-10,
       -10,  5,  5, 10,10,  5,  5,-10,
       -10,  0, 10,10,10, 10,  0,-10,
       -10, 10,10,10,10,10,10,-10,
       -10,  5,  0,  0, 0,  0,  5,-10,
       -20,-10,-10,-10,-10,-10,-10,-20
    },
    // ROOK
    {
         0, 0, 0, 5, 5, 0, 0, 0,
        -5, 0, 0, 0, 0, 0, 0,-5,
        -5, 0, 0, 0, 0, 0, 0,-5,
        -5, 0, 0, 0, 0, 0, 0,-5,
        -5, 0, 0, 0, 0, 0, 0,-5,
        -5, 0, 0, 0, 0, 0, 0,-5,
         5,10,10,10,10,10,10, 5,
         0, 0, 0, 0, 0, 0, 0, 0
    },
    // QUEEN
    {
       -20,-10,-10, -5, -5,-10,-10,-20,
       -10,  0,  0,  0,  0,  0,  0,-10,
       -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
         0,  0,  5,  5,  5,  5,  0, -5,
       -10,  5,  5,  5,  5,  5,  0,-10,
       -10,  0,  5,  0,  0,  0,  0,-10,
       -20,-10,-10, -5, -5,-10,-10,-20
    },
    // KING (midgame)
    {
       -30,-40,-40,-50,-50,-40,-40,-30,
       -30,-40,-40,-50,-50,-40,-40,-30,
       -30,-40,-40,-50,-50,-40,-40,-30,
       -30,-40,-40,-50,-50,-40,-40,-30,
       -20,-30,-30,-40,-40,-30,-30,-20,
       -10,-20,-20,-20,-20,-20,-20,-10,
        20, 20,  0,  0,  0,  0, 20, 20,
        20, 30, 10,  0,  0, 10, 30, 20
    }
};

static const int PST_EG_KING[64] = {
    -50,-30,-30,-30,-30,-30,-30,-50,
    -30,-10,-10,-10,-10,-10,-10,-30,
    -30,-10, 20, 20, 20, 20,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 20, 20, 20, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};

// --- Evaluation ---
int evaluate(const Position& pos, Color perspective) {
    int score = 0;
    int total_material = 0;

    for (int pt = PAWN; pt < KING; ++pt) {
        int wm = pop_count(pos.bitboard_of(WHITE, PieceType(pt))) * piece_value(PieceType(pt));
        int bm = pop_count(pos.bitboard_of(BLACK, PieceType(pt))) * piece_value(PieceType(pt));
        total_material += wm + bm;
    }

    bool endgame = total_material <= 2400;

    for (int pt = PAWN; pt <= KING; ++pt) {
        Bitboard w_bb = pos.bitboard_of(WHITE, PieceType(pt));
        Bitboard b_bb = pos.bitboard_of(BLACK, PieceType(pt));

        while (w_bb) {
            Square sq = pop_lsb(&w_bb);
            score += piece_value(PieceType(pt));
            score += (pt == KING && endgame ? PST_EG_KING[sq] : PST_MG[pt][sq]);
        }

        while (b_bb) {
            Square sq = pop_lsb(&b_bb);
            score -= piece_value(PieceType(pt));
            score -= (pt == KING && endgame ? PST_EG_KING[sq ^ 56] : PST_MG[pt][sq ^ 56]);
        }
    }

    Move dummy[256];
    int white_mob = pos.generate_legals<WHITE>(dummy) - dummy;
    int black_mob = pos.generate_legals<BLACK>(dummy) - dummy;
    score += 5 * (white_mob - black_mob);

    return (perspective == WHITE) ? score : -score;
}

// --- Alpha-Beta Search ---
int alpha_beta(Position& pos, int depth, int alpha, int beta, Color side) {
    if (depth == 0) {
        return evaluate(pos, side);
    }

    Move moves[256];
    Move* end = (side == WHITE) ? pos.generate_legals<WHITE>(moves) : pos.generate_legals<BLACK>(moves);

    if (end == moves) {
        bool check = (side == WHITE) ? pos.in_check<WHITE>() : pos.in_check<BLACK>();
        return check ? -100000 + (3 - depth) : 0;
    }

    int best_score = -1000000;

    for (Move* m = moves; m != end; ++m) {
        if (side == WHITE) pos.play<WHITE>(*m);
        else pos.play<BLACK>(*m);

        int score = -alpha_beta(pos, depth - 1, -beta, -alpha, Color(~side));

        if (side == WHITE) pos.undo<WHITE>(*m);
        else pos.undo<BLACK>(*m);

        best_score = std::max(best_score, score);
        alpha = std::max(alpha, score);
        if (alpha >= beta) break;
    }

    return best_score;
}

Move find_best_move_alpha_beta(Position& pos, Color side, int depth) {
    Move moves[256];
    Move* end = (side == WHITE) ? pos.generate_legals<WHITE>(moves) : pos.generate_legals<BLACK>(moves);

    Move best_move;
    int best_score = -1000000;

    for (Move* m = moves; m != end; ++m) {
        if (side == WHITE) pos.play<WHITE>(*m);
        else pos.play<BLACK>(*m);

        int score = -alpha_beta(pos, depth - 1, -1000000, 1000000, Color(~side));

        if (side == WHITE) pos.undo<WHITE>(*m);
        else pos.undo<BLACK>(*m);

        if (score > best_score) {
            best_score = score;
            best_move = *m;
        }
    }

    return best_move;
}
