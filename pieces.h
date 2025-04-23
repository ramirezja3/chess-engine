// pieces.h
#pragma once

#include "types.h"
#include "position.h"

// Returns the base value of a piece type in centipawns
int piece_value(PieceType pt);

// Evaluates the current position from the perspective of the given color
int evaluate(Position& pos, Color perspective);

int alpha_beta(Position& pos, int depth, int alpha, int beta, Color side);
Move find_best_move_alpha_beta(Position& pos, Color side, int depth);