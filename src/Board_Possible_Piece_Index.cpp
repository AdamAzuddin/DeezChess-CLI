#include <iostream>
#include <vector>
#include "include/Board.h"


int Board::possiblePawnIndex(int targetIndex)
{
    int pawnIndex;
    if (whiteToMove)
    {
        pawnIndex = (targetIndex > 23 && targetIndex < 32) ? targetIndex - 16 : targetIndex - 8;
    }
    else
    {
        pawnIndex = (targetIndex > 31 && targetIndex < 40) ? targetIndex + 16 : targetIndex + 8;
    }
    return pawnIndex;
}

std::vector<int> Board::possibleKnightIndices(int targetIndex)
{
    unsigned long long knightsMask = whiteToMove ? whiteKnights.getBitboard() : blackKnights.getBitboard();

    // find knight attack at the target index;

    Bitboard possibleSquaresToComeFromMask;
    possibleSquaresToComeFromMask.setBitboard(generateKnightAttackMask(targetIndex));
    possibleSquaresToComeFromMask.setBitboard(possibleSquaresToComeFromMask.getBitboard() & knightsMask); // exclude those that no knight exist
    return possibleSquaresToComeFromMask.getIndicesOfPieces(); // get indices of remaining possible square(s)
}

std::vector<int> Board::possibleBishopIndices(int targetIndex)
{
    unsigned long long bishopMask = whiteToMove ? whiteBishops.getBitboard() : blackBishops.getBitboard();

    Bitboard possibleSquaresToComeFromMask;
    possibleSquaresToComeFromMask.setBitboard(generateBishopMovementMask(targetIndex, whiteToMove)); // use movement mask instead of attack mask
    possibleSquaresToComeFromMask.setBitboard(possibleSquaresToComeFromMask.getBitboard() & bishopMask); // exclude those that no bishop exist
    
    return possibleSquaresToComeFromMask.getIndicesOfPieces(); // get indices of remaining possible square(s)
}

std::vector<int> Board::possibleRookIndices(int targetIndex)
{
    unsigned long long rookMask = whiteToMove ? whiteRooks.getBitboard() : blackRooks.getBitboard();

    Bitboard possibleSquaresToComeFromMask;
    possibleSquaresToComeFromMask.setBitboard(generateRookMovementMask(targetIndex, whiteToMove)); // use movement mask instead of attack mask
    possibleSquaresToComeFromMask.setBitboard(possibleSquaresToComeFromMask.getBitboard() & rookMask); // exclude those that no queen exist
    
    return possibleSquaresToComeFromMask.getIndicesOfPieces(); // get indices of remaining possible square(s)
}

std::vector<int> Board::possibleQueenIndices(int targetIndex)
{
    unsigned long long queenMask = whiteToMove ? whiteQueens.getBitboard() : blackQueens.getBitboard();

    whiteQueens.printBitboardInRowsAndColumns();

    Bitboard possibleSquaresToComeFromMask;
    possibleSquaresToComeFromMask.setBitboard(generateQueenMovementMask(targetIndex, whiteToMove) | generateRookMovementMaskQueen(targetIndex, whiteToMove)); // use movement mask instead of attack mask

    possibleSquaresToComeFromMask.setBitboard(possibleSquaresToComeFromMask.getBitboard() & queenMask); // exclude those that no queen exist
    
    return possibleSquaresToComeFromMask.getIndicesOfPieces(); // get indices of remaining possible square(s)
}


