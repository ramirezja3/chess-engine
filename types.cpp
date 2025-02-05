#include "types.h"
#include "iostream"


//file masks
const Bitboard MASK_FILE[8] = {
    0x101010101010101, 0x202020202020202, 0x404040404040404, 0x808080808080808,
	0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080
};

//rank masks
const Bitboard MASK_RANK[8] = {
    0xff, 0xff00, 0xff0000, 0xff000000, 0xff00000000, 0xff0000000000,
    0xff000000000000, 0xff00000000000000
};

//diagonal masks
const Bitboard MASK_DIAGONAL[15] = {
	0x80, 0x8040, 0x804020,
	0x80402010, 0x8040201008, 0x804020100804,
	0x80402010080402, 0x8040201008040201, 0x4020100804020100,
	0x2010080402010000, 0x1008040201000000, 0x804020100000000,
	0x402010000000000, 0x201000000000000, 0x100000000000000,
};

//anti-diagonal masks
const Bitboard MASK_ANTI_DIAGONAL[15] = {
	0x1, 0x102, 0x10204,
	0x1020408, 0x102040810, 0x10204081020,
	0x1020408102040, 0x102040810204080, 0x204081020408000,
	0x408102040800000, 0x810204080000000, 0x1020408000000000,
	0x2040800000000000, 0x4080000000000000, 0x8000000000000000,
};

const Bitboard 

//Prints bitboard, little-endian format
void print_bitboard(Bitboard b) {
	for (int i = 56; i >= 0; i -= 8) {
		for (int j = 0; j < 8; j++)
			std::cout << (char)(((b >> (i + j)) & 1) + '0') << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
}

//Returns number of set bits in the bitboard
inline int pop_count(Bitboard x) {
	x = x - ((x >> 1) & k1);
	x = (x & k2) + ((x >> 2) & k2);
	x = (x + (x >> 4)) & k4;
	x = (x * kf) >> 56;
	return int(x);
}

//Returns number of set bits in the bitboard. Faster than pop_count(x) when the bitboard has few set bits
inline int sparse_pop_count(Bitboard x) {
	int count = 0;
	while (x) {
		count++;
		x &= x - 1;
	}
	return count;
}

const int DEBRUIJN64[64] = {
	0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

const Bitboard MAGIC = 0x03f79d71b4cb0a89;

//Returns the index of the least significant bit in the bitboard, and removes the bit from the bitboard
inline Square pop_lsb(Bitboard* b) {
	int lsb = bsf(*b);
	*b &= *b - 1;
	return Square(lsb);
}

//Returns the index of the least significant bit in the bitboard
constexpr Square bsf(Bitboard b) {
	return Square(DEBRUIJN64[MAGIC * (b ^ (b - 1)) >> 58]);
}

//Returns the representation of the move type in algebraic chess notation. (capture) is used for debugging
const char* MOVE_TYPESTR[16] = {
	"", "", " O-O", " O-O-O", "N", "B", "R", "Q", " (capture)", "", " e.p.", "",
	"N", "B", "R", "Q"
};

//Prints the move
//For example: e5d6 (capture); a7a8R; O-O
std::ostream& operator<<(std::ostream& os, const Move& m) {
	os << SQSTR[m.from()] << SQSTR[m.to()] << MOVE_TYPESTR[m.flags()];
	return os;
}