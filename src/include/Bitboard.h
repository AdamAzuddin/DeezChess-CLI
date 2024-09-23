#ifndef BITBOARD_H
#define BITBOARD_h

#include <vector>

class Bitboard
{
private:
    unsigned long long bitboard;
    bool isWhite;

    

public:
    Bitboard();
    void printBinary();
    void printBitboardInRowsAndColumns();
    unsigned long long getBitboard() const;
    void setBitboard(unsigned long long num);
    void removeAtIndex(int index);
    void addAtIndex(int index);
    void setWhitePiece(bool isWhitePiece);
    bool isWhitePiece();
    bool existAtIndex(int index);
    std::vector<int> getIndicesOfPieces();
    void movePawn(int pawnIndex, int targetIndex);
    void moveRook(int rookIndex, int targetIndex);
    void moveKnight(int knightIndex, int targetIndex);
    void moveBishop(int bishopIndex, int targetIndex);
    void moveQueen(int queenIndex, int targetIndex);
    void moveKing(int kingIndex, int targetIndex);
};

#endif