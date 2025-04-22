// pieces.h
#pragma once

#include "types.h"
#include "position.h"

int piece_value(PieceType pt);
int evaluate(const Position& pos, Color perspective);
int alpha_beta(Position& pos, int depth, int alpha, int beta, Color side);
Move find_best_move_alpha_beta(Position& pos, Color side, int depth);
