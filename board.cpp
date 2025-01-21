

typedef uint64_t U64;

//starting positions
const U64 wPawns   = 0x00ff000000000000ULL
const U64 bPawns   = 0x000000000000ff00ULL
const U64 wRooks   = 0x8100000000000000ULL
const U64 bRooks   = 0x0000000000000081ULL
const U64 wBishops = 0x2400000000000000ULL
const U64 bBishops = 0x0000000000000024ULL
const U64 wKnights = 0x4200000000000000ULL
const U64 bKnights = 0x0000000000000042ULL
const U64 wQueens  = 0x1000000000000000ULL
const U64 bQueens  = 0x0000000000000010ULL
const U64 wKing    = 0x0800000000000000ULL
const U64 bKing    = 0x0000000000000008ULL


#define set_bit(b, i) ((b) |= (1ULL << i))
#define get_bit(b, i) ((b) & (1ULL << i))
#define clear_bit(b, i) ((b) &= ~(1ULL << i))
#define get_LSB(b) (__builtin_ctzll(b))