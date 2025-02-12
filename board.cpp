#include <cstdint>
#include <iostream>


using namespace std;

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

typedef uint64_t

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
      
      U64 empties;
      U64 wBoard;
      U64 bBoard;
      Board(){
        //starting positions
        wPawns   = 0x00ff000000000000ULL;
        bPawns   = 0x000000000000ff00ULL;
        wRooks   = 0x8100000000000000ULL;
        bRooks   = 0x0000000000000081ULL;
        wBishops = 0x2400000000000000ULL;
        bBishops = 0x0000000000000024ULL;
        wKnights = 0x4200000000000000ULL;
        bKnights = 0x0000000000000042ULL;
        wQueens  = 0x1000000000000000ULL;
        bQueens  = 0x0000000000000010ULL;
        wKing    = 0x0800000000000000ULL;
        bKing    = 0x0000000000000008ULL;

        empties  = 0x0000ffffffff0000ULL;
        wBoard   = 0xffff000000000000ULL;
        bBoard   = 0x000000000000ffffULL;
      }
      string getChessMove();
      void setChessMove(string);
};

#define set_bit(b, i) ((b) |= (1ULL << i))
#define get_bit(b, i) ((b) & (1ULL << i))
#define clear_bit(b, i) ((b) &= ~(1ULL << i))
#define get_LSB(b) (__builtin_ctzll(b)) //gets number of trailing zeros

#define knightMoves(b) ()

int pop_LSB(U64 &b) {
    int i = get_LSB(b);
    b &= b - 1;
    return i;
}

string Board::getChessMove()
{
    string move;
    cout << "Enter your move: ";
    cin >> move;
    return move;  
}

void Board::setChessMove(string move)
{
      cout << move << endl;
}

int main()
{
    Board play;
    play.getChessMove();
}

