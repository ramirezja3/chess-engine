
const U64 fileA = 0x8080808080808080ULL;
const U64 fileB = 0x4040404040404040ULL;
const U64 fileC = 0x2020202020202020ULL;
const U64 fileD = 0x1010101010101010ULL;
const U64 fileE = 0x0808080808080808ULL;
const U64 fileF = 0x0404040404040404ULL;
const U64 fileG = 0x0202020202020202ULL;
const U64 fileH = 0x0101010101010101ULL;

const U64 rank1 = 0xff00000000000000ULL;
const U64 rank2 = 0x00ff000000000000ULL;
const U64 rank3 = 0x0000ff0000000000ULL;
const U64 rank4 = 0x000000ff00000000ULL;
const U64 rank5 = 0x00000000ff000000ULL;
const U64 rank6 = 0x0000000000ff0000ULL;
const U64 rank7 = 0x000000000000ff00ULL;
const U64 rank8 = 0x00000000000000ffULL;

typedef uint64_t U64;

class Board {
    public:
      //hold space for bitwise positions
      U64 wPawns;
      U64 bPawns;
      U64 wRooks;
      U64 bRooks;
      U64 wKnights;
      U64 bKnights;
      U64 wBishops;
      U64 bBishops;
      U64 wQueens;
      U64 bQueens;
      U64 wKing;
      U64 bKing;
      Board(){
        //starting positions
        U64 wPawns   = 0x00ff000000000000ULL;
        U64 bPawns   = 0x000000000000ff00ULL;
        U64 wRooks   = 0x8100000000000000ULL;
        U64 bRooks   = 0x0000000000000081ULL;
        U64 wBishops = 0x2400000000000000ULL;
        U64 bBishops = 0x0000000000000024ULL;
        U64 wKnights = 0x4200000000000000ULL;
        U64 bKnights = 0x0000000000000042ULL;
        U64 wQueens  = 0x1000000000000000ULL;
        U64 bQueens  = 0x0000000000000010ULL;
        U64 wKing    = 0x0800000000000000ULL;
        U64 bKing    = 0x0000000000000008ULL;
      }
};

#define set_bit(b, i) ((b) |= (1ULL << i))
#define get_bit(b, i) ((b) & (1ULL << i))
#define clear_bit(b, i) ((b) &= ~(1ULL << i))
#define get_LSB(b) (__builtin_ctzll(b)) //gets number of trailing zeros

int pop_LSB(U64 &b) {
    int i = get_LSB(b);
    b &= b - 1;
    return i;
}