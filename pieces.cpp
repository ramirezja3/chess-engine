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

const double blackPawnPosition[64] = {
     20, 20, 20, 20, 20, 20, 20, 20,
     25, 30, 30,  0,  0, 30, 30, 25,
     25, 15, 10, 20, 20, 10, 15, 25,
     20, 20, 20, 40, 40, 20, 20, 20,
     25, 25, 30, 45, 45, 30, 25, 25,
     30, 30, 40, 50, 50, 40, 30, 30,
     70, 70, 70, 70, 70, 70, 70, 70,
     20, 20, 20, 20, 20, 20, 20, 20
};

const double whitePawnPosition[64] = {
     20, 20, 20, 20, 20, 20, 20, 20,
     70, 70, 70, 70, 70, 70, 70, 70,
     30, 30, 40, 50, 50, 40, 30, 30,
     25, 25, 30, 45, 45, 30, 25, 25,
     20, 20, 20, 40, 40, 20, 20, 20,
     25, 15, 10, 20, 20, 10, 15, 25,
     25, 30, 30,  0,  0, 30, 30, 25,
     20, 20, 20, 20, 20, 20, 20, 20
};

const double blackKnightPosition[64] = {
      0, 10, 20, 20, 20, 20, 10,  0,
     10, 30, 50, 55, 55, 50, 30, 10,
     20, 55, 60, 65, 65, 60, 55, 20,
     20, 50, 65, 70, 70, 65, 50, 20,
     20, 55, 65, 70, 70, 65, 55, 20,
     20, 50, 60, 65, 65, 60, 50, 20,
     10, 30, 50, 50, 50, 50, 30, 10,
      0, 10, 20, 20, 20, 20, 10,  0
};

const double whiteKnightPosition[64] = {
      0, 10, 20, 20, 20, 20, 10,  0,
     10, 30, 50, 50, 50, 50, 30, 10,
     20, 50, 60, 65, 65, 60, 50, 20,
     20, 55, 65, 70, 70, 65, 55, 20,
     20, 50, 65, 70, 70, 65, 50, 20,
     20, 55, 60, 65, 65, 60, 55, 20,
     10, 30, 50, 55, 55, 50, 30, 10,
      0, 10, 20, 20, 20, 20, 10,  0
};

const double blackBishopPosition[64] = {
      0, 10, 10, 10, 10, 10, 10,  0,
     10, 25, 20, 20, 20, 20, 25, 10,
     10, 30, 30, 30, 30, 30, 30, 10,
     10, 20, 30, 30, 30, 30, 20, 10,
     10, 25, 25, 30, 30, 25, 25, 10,
     10, 20, 25, 30, 30, 25, 20, 10,
     10, 20, 20, 20, 20, 20, 20, 10,
      0, 10, 10, 10, 10, 10, 10,  0
};

const double whiteBishopPosition[64] = {
      0, 10, 10, 10, 10, 10, 10,  0,
     10, 20, 20, 20, 20, 20, 20, 10,
     10, 20, 25, 30, 30, 25, 20, 10,
     10, 25, 25, 30, 30, 25, 25, 10,
     10, 20, 30, 30, 30, 30, 20, 10,
     10, 30, 30, 30, 30, 30, 30, 10,
     10, 25, 20, 20, 20, 20, 25, 10,
      0, 10, 10, 10, 10, 10, 10,  0
};

const double blackRookPosition[64] = {
      5,  5,  5, 10, 10,  5,  5,  5,
      0,  5,  5,  5,  5,  5,  5,  0,
      0,  5,  5,  5,  5,  5,  5,  0,
      0,  5,  5,  5,  5,  5,  5,  0,
      0,  5,  5,  5,  5,  5,  5,  0,
      0,  5,  5,  5,  5,  5,  5,  0,
     10, 15, 15, 15, 15, 15, 15, 10,
      5,  5,  5,  5,  5,  5,  5,  5
};

const double whiteRookPosition[64] = {
      5,  5,  5,  5,  5,  5,  5,  5,
     10, 15, 15, 15, 15, 15, 15, 10,
      0,  5,  5,  5,  5,  5,  5,  0,
      0,  5,  5,  5,  5,  5,  5,  0,
      0,  5,  5,  5,  5,  5,  5,  0,
      0,  5,  5,  5,  5,  5,  5,  0,
      0,  5,  5,  5,  5,  5,  5,  0,
      5,  5,  5, 10, 10,  5,  5,  5
};

const double blackQueenPosition[64] = {
      0, 10, 10, 15, 15, 10, 10,  0,
     10, 20, 20, 20, 20, 20, 20, 10,
     10, 25, 25, 25, 25, 25, 20, 10,
     20, 20, 25, 25, 25, 25, 20, 15,
     15, 20, 25, 25, 25, 25, 20, 15,
     10, 20, 25, 25, 25, 25, 20, 10,
     10, 20, 20, 20, 20, 20, 20, 10,
      0, 10, 10, 15, 15, 10, 10,  0
};

const double whiteQueenPosition[64] = {
      0, 10, 10, 15, 15, 10, 10,  0,
     10, 20, 20, 20, 20, 20, 20, 10,
     10, 20, 25, 25, 25, 25, 20, 10,
     15, 20, 25, 25, 25, 25, 20, 15,
     20, 20, 25, 25, 25, 25, 20, 15,
     10, 25, 25, 25, 25, 25, 20, 10,
     10, 20, 20, 20, 20, 20, 20, 10,
      0, 10, 10, 15, 15, 10, 10,  0
};

const double blackKingPosition[64] = {
      50, 60, 40, 30, 30, 40, 60, 50,
      50, 50, 30, 30, 30, 30, 50, 50,
      20, 10, 10, 10, 10, 10, 10, 20,
      10,  0,  0, -10, -10,  0,  0, 10,
      0, -10, -10, -20, -20, -10, -10,  0,
      0, -10, -10, -20, -20, -10, -10,  0,
      0, -10, -10, -20, -20, -10, -10,  0,
      0, -10, -10, -20, -20, -10, -10,  0
};

const double whiteKingPosition[64] = {
      0, -10, -10, -20, -20, -10, -10,  0,
      0, -10, -10, -20, -20, -10, -10,  0,
      0, -10, -10, -20, -20, -10, -10,  0,
      0, -10, -10, -20, -20, -10, -10,  0,
     10,  0,  0, -10, -10,  0,  0, 10,
     20, 10, 10, 10, 10, 10, 10, 20,
     50, 50, 30, 30, 30, 30, 50, 50,
     50, 60, 40, 30, 30, 40, 60, 50
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

double getEval()
{
    double whiteEval = getWhiteEval();
    double blackEval = getBlackEval();

//     if (whiteEval == blackEval) return 0.5f;
//     float advantage = whiteEval / (std::abs(whiteEval) + std::abs(blackEval));
//     (advantage + 1.0f) / 2.0f;
     double advantage = 

    
}



int main()
{
     std::cout << getWhiteEval() << std::endl;
     std::cout << getBlackEval() << std::endl;
     std::cout << getEval() << std::endl;
     return 0;
}
