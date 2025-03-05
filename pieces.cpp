#include <cstdint>
#include <iostream>

// int wPieces[10];
// int bPieces[10];

const int pawn = 10;
const int knight = 32;
const int bishop = 33;
const int rook = 50;
const int queen = 90;
const int king = 200;

const int blackPawnPosition[64] = {
      0,  0,  0,  0,  0,  0,  0,  0 ,
      5, 10, 10,-20,-20, 10, 10,  5 ,
      5, -5,-10,  0,  0,-10, -5,  5 ,
      0,  0,  0, 20, 20,  0,  0,  0 ,
      5,  5, 10, 25, 25, 10,  5,  5 ,
     10, 10, 20, 30, 30, 20, 10, 10 ,
     50, 50, 50, 50, 50, 50, 50, 50 ,
      0,  0,  0,  0,  0,  0,  0,  0 
};

const int whitePawnPosition[64] = {
      0,  0,  0,  0,  0,  0,  0,  0 ,
      50, 50, 50, 50, 50, 50, 50, 50 , 
      10, 10, 20, 30, 30, 20, 10, 10 , 
      5,  5, 10, 25, 25, 10,  5,  5 , 
      0,  0,  0, 20, 20,  0,  0,  0 ,
     5, -5,-10,  0,  0,-10, -5,  5 ,
      5, 10, 10,-20,-20, 10, 10,  5,
      0,  0,  0,  0,  0,  0,  0,  0 
};

const int blackKnightPosition[64] = {
     -50,-40,-30,-30,-30,-30,-40,-50 ,
     -40,-20,  0,  5,  5,  0,-20,-40 ,
     -30,  5, 10, 15, 15, 10,  5,-30 ,
     -30,  0, 15, 20, 20, 15,  0,-30 ,
     -30,  5, 15, 20, 20, 15,  5,-30 ,
     -30,  0, 10, 15, 15, 10,  0,-30 ,
     -40,-20,  0,  0,  0,  0,-20,-40 ,
     -50,-40,-30,-30,-30,-30,-40,-50 
};

const int whiteKnightPosition[64] = {
     -50,-40,-30,-30,-30,-30,-40,-50 ,
     -40,-20,  0,  0,  0,  0,-20,-40 , 
     -30,  0, 10, 15, 15, 10,  0,-30 , 
     -30,  5, 15, 20, 20, 15,  5,-30 ,
     -30,  0, 15, 20, 20, 15,  0,-30 ,
     -30,  5, 10, 15, 15, 10,  5,-30 ,
     -40,-20,  0,  5,  5,  0,-20,-40 ,
     -50,-40,-30,-30,-30,-30,-40,-50 
};

const int blackBishopPosition[64] = {
     -20,-10,-10,-10,-10,-10,-10,-20 ,
     -10,  5,  0,  0,  0,  0,  5,-10 ,
     -10, 10, 10, 10, 10, 10, 10,-10 ,
     -10,  0, 10, 10, 10, 10,  0,-10 ,
     -10,  5,  5, 10, 10,  5,  5,-10 ,
     -10,  0,  5, 10, 10,  5,  0,-10 ,
     -10,  0,  0,  0,  0,  0,  0,-10 ,
     -20,-10,-10,-10,-10,-10,-10,-20 
};

const int whiteBishopPosition[64] = {
     -20,-10,-10,-10,-10,-10,-10,-20 ,
     -10,  0,  0,  0,  0,  0,  0,-10 ,
     -10,  0,  5, 10, 10,  5,  0,-10 , 
     -10,  5,  5, 10, 10,  5,  5,-10 ,
     -10,  0, 10, 10, 10, 10,  0,-10 ,
     -10, 10, 10, 10, 10, 10, 10,-10 ,
     -10,  5,  0,  0,  0,  0,  5,-10 ,
     -20,-10,-10,-10,-10,-10,-10,-20 
};

const int blackRookPosition[64] = {
      0,  0,  0,  5,  5,  0,  0,  0 ,
     -5,  0,  0,  0,  0,  0,  0, -5 ,
     -5,  0,  0,  0,  0,  0,  0, -5 ,
     -5,  0,  0,  0,  0,  0,  0, -5 ,
     -5,  0,  0,  0,  0,  0,  0, -5 ,
     -5,  0,  0,  0,  0,  0,  0, -5 ,
      5, 10, 10, 10, 10, 10, 10,  5 ,
      0,  0,  0,  0,  0,  0,  0,  0 
};

const int whiteRookPosition[64] = {
      0,  0,  0,  0,  0,  0,  0,  0  ,
     5, 10, 10, 10, 10, 10, 10,  5 ,
     -5,  0,  0,  0,  0,  0,  0, -5 ,
     -5,  0,  0,  0,  0,  0,  0, -5 ,
     -5,  0,  0,  0,  0,  0,  0, -5 ,
     -5,  0,  0,  0,  0,  0,  0, -5 ,
      -5,  0,  0,  0,  0,  0,  0, -5 ,
      0,  0,  0,  5,  5,  0,  0,  0 
};

const int blackQueenPosition[64] = {
     -20,-10,-10, -5, -5,-10,-10,-20 ,
     -10,  0,  0,  0,  0,  0,  0,-10 ,
     -10,  5,  5,  5,  5,  5,  0,-10 ,
      0,  0,  5,  5,  5,  5,  0, -5 ,
     -5,  0,  5,  5,  5,  5,  0, -5 ,
     -10,  0,  5,  5,  5,  5,  0,-10 ,
     -10,  0,  0,  0,  0,  0,  0,-10 ,
     -20,-10,-10, -5, -5,-10,-10,-20 
};

const int whiteQueenPosition[64] = {
     -20,-10,-10, -5, -5,-10,-10,-20 ,
     -10,  0,  0,  0,  0,  0,  0,-10 ,
     -10,  0,  5,  5,  5,  5,  0,-10 ,
      -5,  0,  5,  5,  5,  5,  0, -5  ,
     0,  0,  5,  5,  5,  5,  0, -5 ,
     -10,  5,  5,  5,  5,  5,  0,-10 ,
     -10,  0,  0,  0,  0,  0,  0,-10 ,
     -20,-10,-10, -5, -5,-10,-10,-20 
};


const int blackKingPosition[64] = {
      20, 30, 10,  0,  0, 10, 30, 20 ,
      20, 20,  0,  0,  0,  0, 20, 20 ,
     -10,-20,-20,-20,-20,-20,-20,-10 ,
     -20,-30,-30,-40,-40,-30,-30,-20 ,
     -30,-40,-40,-50,-50,-40,-40,-30 ,
     -30,-40,-40,-50,-50,-40,-40,-30 ,
     -30,-40,-40,-50,-50,-40,-40,-30 ,
     -30,-40,-40,-50,-50,-40,-40,-30 
};

const int whiteKingPosition[64] = {
      -30,-40,-40,-50,-50,-40,-40,-30 ,
      -30,-40,-40,-50,-50,-40,-40,-30 ,
     -30,-40,-40,-50,-50,-40,-40,-30 ,
     -30,-40,-40,-50,-50,-40,-40,-30 ,
     -20,-30,-30,-40,-40,-30,-30,-20  ,
     -10,-20,-20,-20,-20,-20,-20,-10 ,
     20, 20,  0,  0,  0,  0, 20, 20 ,
     20, 30, 10,  0,  0, 10, 30, 20
};

int getWhiteEval()
{
     //  int pawnTotal = pawn * whitePawnPosition[33] + pawn * whitePawnPosition[43] + pawn * whitePawnPosition[46];
     //  int knightTotal = knight * whiteKnightPosition[56];
     //  int kingTotal = king * whiteKingPosition[61];
     //  int queenTotal = queen * whiteQueenPosition[60];
     //  int bishopTotal = bishop * whiteBishopPosition[24];
     //  int total = pawnTotal + knightTotal + kingTotal + queenTotal + bishopTotal;

     int pawnTotal = pawn * whitePawnPosition[40] + pawn * whitePawnPosition[33];
     int knightTotal = knight * whiteKnightPosition[18];
     int bishopTotal = bishop * whiteBishopPosition[42];
     int kingTotal = king * whiteKingPosition[25];
     int rookTotal = rook * whiteRookPosition[63];
     int total = pawnTotal + knightTotal + kingTotal + rookTotal + bishopTotal;
     return total;
}

int getBlackEval()
{
     // int pawnTotal = pawn * blackBishopPosition[13] + pawn * blackPawnPosition[22];
     // int knightTotal = knight * blackKnightPosition[28];
     // int kingTotal = king * blackKingPosition[14];
     // int queenTotal = queen * blackQueenPosition[30];
     // int bishopTotal = bishop * blackBishopPosition[18];
     // int total = pawnTotal + knightTotal + kingTotal + queenTotal + bishopTotal;

     int pawnTotal = pawn * blackBishopPosition[29] + pawn * blackPawnPosition[55];
     int rookTotal = rook * blackRookPosition[44] + rook * blackRookPosition[52];
     int kingTotal = king * blackKingPosition[22];
     int total = pawnTotal + kingTotal + rookTotal;
     return total;
}

float getEval()
{
    float whiteEval = getWhiteEval();
    float blackEval = getBlackEval();

    if (whiteEval == blackEval) return 0.5f;
    float advantage = whiteEval / (std::abs(whiteEval) + std::abs(blackEval));
    return (advantage + 1.0f) / 2.0f;
}



int main()
{
     std::cout << getWhiteEval() << std::endl;
     std::cout << getBlackEval() << std::endl;
     std::cout << getEval() << std::endl;
     return 0;
}
