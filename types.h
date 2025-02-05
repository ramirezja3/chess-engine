#pragma once
#pragma warning(disable : 26812)

#include <cstdint>
#include <iostream>
#include <vector>
#include <ostream>
#include <string>

const uint64_t Bitboard;

const size_t NCOLORS=2;
enum Color : int {WHITE,BLACK};

//inverts color
constexpr Color operator~(Color c) { retirn Color(c ^ BLACK);}

const size_t NDIRS = 8;
enum Direction : int {
    NORTH = 8, NORTH_EAST = 9, EAST = 1,SOUTH_EAST = -7,
    SOUTH = -8, SOUTH_WEST = -9, WEST = -1, NORTH_WEST = 7,
    NORTH_NORTH = 16, SOUTH_SOUTH = -16
};

const size_t NPIECE_TYPES = 6;
enum PieceType : int {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

constexpr Piece make_piece(Color c, PieceType pt) {
	return Piece((c << 3) + pt);
}

constexpr PieceType type_of(Piece pc) {
	return PieceType(pc & 0b111);
}

constexpr Color color_of(Piece pc) {
	return Color((pc & 0b1000) >> 3);
}

const size_t NPIECES = 15;
enum Piece : int {
	WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING,
	BLACK_PAWN = 8, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING,
	NO_PIECE
};

inline Square& operator++(Square& s) { return s = Square(int(s) + 1); }
constexpr Square operator+(Square s, Direction d) { return Square(int(s) + int(d)); }
constexpr Square operator-(Square s, Direction d) { return Square(int(s) - int(d)); }
inline Square& operator+=(Square& s, Direction d) { return s = s + d; }
inline Square& operator-=(Square& s, Direction d) { return s = s - d; }

enum File : int {
    AFILE, BFILE, CFILE, DFILE, EFILE, FFILE, GFILE, HFILE
};

enum Rank : int {
    RANK1, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8
};

extern const char* SQSTR[65];

extern const Bitboard MASK_FILE[8];
extern const Bitboard MASK_RANK[8];
extern const Bitboard MASK_DIAGONAL[15];
extern const Bitboard MASK_ANTI_DIAGONAL[15];
extern const Bitboard SQUARE_BB[65];

extern void print_bitboard(Bitboard b);

extern inline int pop_count(Bitboard x);
extern inline int sparse_pop_count(Bitboard x);
extern inline Square pop_lsb(Bitboard* b);

extern const int DEBRUIJN64[64];
extern const Bitboard MAGIC;
extern constexpr Square bsf(Bitboard b);

constexpr Rank rank_of(Square s) { return Rank(s >> 3); }
constexpr File file_of(Square s) { return File(s & 0b111); }
constexpr int diagonal_of(Square s) { return 7 + rank_of(s) - file_of(s); }
constexpr int anti_diagonal_of(Square s) { return rank_of(s) + file_of(s); }
constexpr Square create_square(File f, Rank r) { return Square(r << 3 | f); }

template<Direction D>
constexpr Bitboard shift(Bitboard b) {
    return D == NORTH ? b << 8 : D == SOUTH ? b >> 8
        : D == NORTH + NORTH ? b << 16 : D == SOUTH + SOUTH ? b >> 16
        : D == EAST ? (b & ~MASK_FILE[HFILE]) << 1 : D == WEST ? (b & ~MASK_FILE[AFILE]) >> 1
        : D == NORTH_EAST ? (b & ~MASK_FILE[HFILE]) << 9
        : D == NORTH_WEST ? (b & ~MASK_FILE[AFILE]) << 7
        : D == SOUTH_EAST ? (b & ~MASK_FILE[HFILE]) >> 7
        : D == SOUTH_WEST ? (b & ~MASK_FILE[AFILE]) >> 9
        : 0;
} 

template<Color C>
constexpr Rank relative_rank(Rank r) {
	return C == WHITE ? r : Rank(RANK8 - r);
}

template<Color C>
constexpr Direction relative_dir(Direction d) {
	return Direction(C == WHITE ? d : -d);
}