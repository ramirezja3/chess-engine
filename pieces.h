// pieces.h
#pragma once

#include "types.h"
#include "position.h"

// Returns the base value of a piece type in centipawns
int piece_value(PieceType pt);

// Evaluates the current position from the perspective of the given color
int evaluate(const Position& pos, Color perspective);
Move find_best_move_alpha_beta(Position& pos, Color side, int depth);