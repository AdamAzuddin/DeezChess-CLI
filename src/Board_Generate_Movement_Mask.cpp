#include <iostream>
#include <vector>
#include "include/Board.h"

// for detecting movement of bishop.
// for example, when finding if a bishop could move to a certain target index, use this function instead of the generate attack mask, since this include the friendly bishop or queen before stopping

unsigned long long Board::generateTopHalfDiagonalMovement(int index, int shift, bool isWhiteBishop)
{
    unsigned long long currentMask;
    int currentIndex = index;
    Bitboard result;
    auto &friendlyPieces = isWhiteBishop ? whitePieces : blackPieces;
    auto &enemyPieces = isWhiteBishop ? blackPieces : whitePieces;
    auto &friendlyBishops = isWhiteBishop ? whiteBishops : blackBishops;
    result.setBitboard(0);

    unsigned long long leftRightTopEdgeMask = FILE_A_MASK | FILE_H_MASK | RANK_8_MASK;

    currentMask = 1ULL << index;

    while (true)
    {
        currentIndex += shift;
        currentMask = 1ULL << currentIndex;

        if (currentMask & friendlyBishops.getBitboard())// added this lines
        {
            result.setBitboard(result.getBitboard() | currentMask);
            break;
        }

        if (currentMask & friendlyPieces.getBitboard() || currentIndex > 63)
        {
            break;
        }

        if (currentMask & leftRightTopEdgeMask || currentMask & enemyPieces.getBitboard())
        {
            result.setBitboard(result.getBitboard() | currentMask);
            break;
        }
        if (currentIndex < 0)
            break;

        result.setBitboard(result.getBitboard() | currentMask);
    }

    return result.getBitboard();
}

unsigned long long Board::generateBottomHalfDiagonalMovement(int index, int shift, bool isWhiteBishop)
{
    unsigned long long currentMask;
    int currentIndex = index;
    Bitboard result;
    auto &friendlyPieces = isWhiteBishop ? whitePieces : blackPieces;
    auto &enemyPieces = isWhiteBishop ? blackPieces : whitePieces;
    auto &friendlyBishops = isWhiteBishop ? whiteBishops : blackBishops;
    result.setBitboard(0);

    unsigned long long leftRightBottomEdgeMask = FILE_A_MASK | FILE_H_MASK | RANK_1_MASK;

    currentMask = 1ULL << index;

    while (true)
    {

        currentIndex -= shift;

        currentMask = 1ULL << currentIndex;

        if (currentMask & friendlyBishops.getBitboard())
        {
            std::cout << "Stopped because detected friendly bishop";
            result.setBitboard(result.getBitboard() | currentMask);
            break;
        }

        if (currentMask & friendlyPieces.getBitboard() || currentIndex < 0)
        {
            break;
        }

        if (currentMask & leftRightBottomEdgeMask || currentMask & enemyPieces.getBitboard())
        {
            result.setBitboard(result.getBitboard() | currentMask);
            break;
        }

        if (currentIndex < 0)
            break;

        result.setBitboard(result.getBitboard() | currentMask);
    }

    return result.getBitboard();
}

unsigned long long Board::generateBishopMovementMask(int bishopIndex, bool isWhiteBishop)
{
    Bitboard attacks;

    // create mask for positive gradient diagonal
    attacks.setBitboard(generateBottomHalfDiagonalMovement(bishopIndex, 7, isWhiteBishop));
    attacks.setBitboard(attacks.getBitboard() | generateBottomHalfDiagonalMovement(bishopIndex, 9, isWhiteBishop));
    attacks.setBitboard(attacks.getBitboard() | generateTopHalfDiagonalMovement(bishopIndex, 9, isWhiteBishop));
    attacks.setBitboard(attacks.getBitboard() | generateTopHalfDiagonalMovement(bishopIndex, 7, isWhiteBishop));

    // create mask for negative gradient diagonal
    return attacks.getBitboard();
}


unsigned long long Board::generateTopHalfDiagonalMovementQueen(int index, int shift, bool isWhiteQueen)
{
    unsigned long long currentMask;
    int currentIndex = index;
    Bitboard result;
    auto &friendlyPieces = isWhiteQueen ? whitePieces : blackPieces;
    auto &enemyPieces = isWhiteQueen ? blackPieces : whitePieces;
    auto &friendlyQueens = isWhiteQueen ? whiteQueens : blackQueens;
    result.setBitboard(0);

    unsigned long long leftRightTopEdgeMask = FILE_A_MASK | FILE_H_MASK | RANK_8_MASK;

    currentMask = 1ULL << index;

    while (true)
    {
        currentIndex += shift;
        currentMask = 1ULL << currentIndex;

        if (currentMask & friendlyQueens.getBitboard())// added this lines
        {
            result.setBitboard(result.getBitboard() | currentMask);
            break;
        }

        if (currentMask & friendlyPieces.getBitboard() || currentIndex > 63)
        {
            break;
        }

        if (currentMask & leftRightTopEdgeMask || currentMask & enemyPieces.getBitboard())
        {
            result.setBitboard(result.getBitboard() | currentMask);
            break;
        }

        result.setBitboard(result.getBitboard() | currentMask);
    }

    return result.getBitboard();
}

unsigned long long Board::generateBottomHalfDiagonalMovementQueen(int index, int shift, bool isWhiteQueen)
{
    unsigned long long currentMask;
    int currentIndex = index;
    Bitboard result;
    auto &friendlyPieces = isWhiteQueen ? whitePieces : blackPieces;
    auto &enemyPieces = isWhiteQueen ? blackPieces : whitePieces;
    auto &friendlyQueens = isWhiteQueen ? whiteQueens : blackQueens;
    result.setBitboard(0);
    friendlyQueens.getIndicesOfPieces();

    unsigned long long leftRightBottomEdgeMask = FILE_A_MASK | FILE_H_MASK | RANK_1_MASK;

    currentMask = 1ULL << index;

    while (true)
    {

        currentIndex -= shift;

        currentMask = 1ULL << currentIndex;

        if (currentMask & friendlyQueens.getBitboard())
        {
            result.setBitboard(result.getBitboard() | currentMask);
            break;
        }

        if (currentMask & friendlyPieces.getBitboard() || currentIndex < 0)
        {
            break;
        }

        if (currentMask & leftRightBottomEdgeMask || currentMask & enemyPieces.getBitboard())
        {
            result.setBitboard(result.getBitboard() | currentMask);
            break;
        }

        result.setBitboard(result.getBitboard() | currentMask);
    }

    return result.getBitboard();
}


unsigned long long Board::generateRookMovementMask(int rookIndex, bool isWhiteRook)
{
    Bitboard fileMask;
    fileMask.setBitboard(0);

    Bitboard rankMask;
    rankMask.setBitboard(0);

    Bitboard attacks;

    unsigned long long rookMask = 1ULL << rookIndex;
    unsigned long long currentMask = 1ULL << rookIndex;
    unsigned long long northMask, southMask, westMask, eastMask = 1ULL << rookIndex;
    unsigned long long FILE_AH_MASK = FILE_A_MASK | FILE_H_MASK;
    auto& friendlyPieces = isWhiteRook ? whitePieces : blackPieces;
    auto& enemyPieces = isWhiteRook ? blackPieces : whitePieces;
    auto& friendlyRooks = isWhiteRook ? whiteRooks : blackRooks;

    int currentIndex = rookIndex;

    unsigned long long rookAttackMask = 0;

    while (true)
    {
        currentIndex += 8;

        if (currentIndex > 63)
            break;

        northMask = 1ULL << currentIndex;

        if (northMask & friendlyRooks.getBitboard())
        {
            fileMask.setBitboard(fileMask.getBitboard() | northMask);
            break;
        }

        if (northMask & friendlyPieces.getBitboard())
            break;

        if (northMask & enemyPieces.getBitboard())
        {
            fileMask.setBitboard(fileMask.getBitboard() | northMask);
            break;
        }
        fileMask.setBitboard(fileMask.getBitboard() | northMask);
    }
    currentIndex = rookIndex;

    while (true)
    {

        currentIndex -= 8;
        if (currentIndex < 0)
        {
            break;
        }

        southMask = 1ULL << currentIndex;
        
        if (southMask & friendlyRooks.getBitboard())
        {
            fileMask.setBitboard(fileMask.getBitboard() | southMask);
            break;
        }
        if (southMask & enemyPieces.getBitboard())
        {
            fileMask.setBitboard(fileMask.getBitboard() | southMask);
            break;
        }

        if (southMask & friendlyPieces.getBitboard())
            break;
        fileMask.setBitboard(fileMask.getBitboard() | southMask);
    }
    currentIndex = rookIndex;

    while (true)
    {
        currentIndex++;
        if (currentIndex > 63)
            break;
        eastMask = 1ULL << currentIndex;

        if (eastMask & friendlyRooks.getBitboard())
        {
            rankMask.setBitboard(rankMask.getBitboard() | eastMask);
            break;
        }

        if (eastMask & FILE_H_MASK)
        {
            rankMask.setBitboard(rankMask.getBitboard() | eastMask);
            break;
        }

        if (eastMask & friendlyPieces.getBitboard())
            break;

        if (eastMask & enemyPieces.getBitboard())
        {
            rankMask.setBitboard(rankMask.getBitboard() | eastMask);
            break;
        }
        rankMask.setBitboard(rankMask.getBitboard() | eastMask);
    }

    currentIndex = rookIndex;

    while (true)
    {
        currentIndex--;
        if (currentIndex < 0)
            break;
        westMask = 1ULL << currentIndex;

        if (westMask & friendlyRooks.getBitboard())
        {
            rankMask.setBitboard(rankMask.getBitboard() | westMask);
            break;
        }

        if (westMask & FILE_A_MASK)
        {
            rankMask.setBitboard(rankMask.getBitboard() | westMask);
            break;
        }

        if (westMask & friendlyPieces.getBitboard())
            break;

        if (westMask & enemyPieces.getBitboard())
        {
            rankMask.setBitboard(rankMask.getBitboard() | westMask);
            break;
        }
        rankMask.setBitboard(rankMask.getBitboard() | westMask);
    }

    return fileMask.getBitboard() | rankMask.getBitboard();
}



unsigned long long Board::generateRookMovementMaskQueen(int queenIndex, bool isWhiteQueen)
{
    Bitboard fileMask;
    fileMask.setBitboard(0);

    Bitboard rankMask;
    rankMask.setBitboard(0);

    Bitboard attacks;

    unsigned long long rookMask = 1ULL << queenIndex;
    unsigned long long currentMask = 1ULL << queenIndex;
    unsigned long long northMask, southMask, westMask, eastMask = 1ULL << queenIndex;
    unsigned long long FILE_AH_MASK = FILE_A_MASK | FILE_H_MASK;
    auto& friendlyPieces = isWhiteQueen ? whitePieces : blackPieces;
    auto& enemyPieces = isWhiteQueen ? blackPieces : whitePieces;
    auto& friendlyQueens = isWhiteQueen ? whiteQueens : blackQueens;

    int currentIndex = queenIndex;

    unsigned long long rookAttackMask = 0;

    while (true)
    {
        currentIndex += 8;

        if (currentIndex > 63)
            break;

        northMask = 1ULL << currentIndex;

        if (northMask & friendlyQueens.getBitboard())
        {
            fileMask.setBitboard(fileMask.getBitboard() | northMask);
            break;
        }

        if (northMask & friendlyPieces.getBitboard())
            break;

        if (northMask & enemyPieces.getBitboard())
        {
            fileMask.setBitboard(fileMask.getBitboard() | northMask);
            break;
        }
        fileMask.setBitboard(fileMask.getBitboard() | northMask);
    }
    currentIndex = queenIndex;

    while (true)
    {

        currentIndex -= 8;
        if (currentIndex < 0)
        {
            break;
        }

        southMask = 1ULL << currentIndex;
        
        if (southMask & friendlyQueens.getBitboard())
        {
            fileMask.setBitboard(fileMask.getBitboard() | southMask);
            break;
        }
        if (southMask & enemyPieces.getBitboard())
        {
            fileMask.setBitboard(fileMask.getBitboard() | southMask);
            break;
        }

        if (southMask & friendlyPieces.getBitboard())
            break;
        fileMask.setBitboard(fileMask.getBitboard() | southMask);
    }
    currentIndex = queenIndex;

    while (true)
    {
        currentIndex++;
        if (currentIndex > 63)
            break;
        eastMask = 1ULL << currentIndex;

        if (eastMask & friendlyQueens.getBitboard())
        {
            rankMask.setBitboard(rankMask.getBitboard() | eastMask);
            break;
        }

        if (eastMask & FILE_H_MASK)
        {
            rankMask.setBitboard(rankMask.getBitboard() | eastMask);
            break;
        }

        if (eastMask & friendlyPieces.getBitboard())
            break;

        if (eastMask & enemyPieces.getBitboard())
        {
            rankMask.setBitboard(rankMask.getBitboard() | eastMask);
            break;
        }
        rankMask.setBitboard(rankMask.getBitboard() | eastMask);
    }

    currentIndex = queenIndex;

    while (true)
    {
        currentIndex--;
        if (currentIndex < 0)
            break;
        westMask = 1ULL << currentIndex;

        if (westMask & friendlyQueens.getBitboard())
        {
            rankMask.setBitboard(rankMask.getBitboard() | westMask);
            break;
        }

        if (westMask & FILE_A_MASK)
        {
            rankMask.setBitboard(rankMask.getBitboard() | westMask);
            break;
        }

        if (westMask & friendlyPieces.getBitboard())
            break;

        if (westMask & enemyPieces.getBitboard())
        {
            rankMask.setBitboard(rankMask.getBitboard() | westMask);
            break;
        }
        rankMask.setBitboard(rankMask.getBitboard() | westMask);
    }

    return fileMask.getBitboard() | rankMask.getBitboard();
}

unsigned long long Board::generateQueenMovementMask(int queenIndex, bool isWhiteQueen)
{
    Bitboard attacks;

    // create mask for positive gradient diagonal
    attacks.setBitboard(generateBottomHalfDiagonalMovementQueen(queenIndex, 7, isWhiteQueen));
    attacks.setBitboard(attacks.getBitboard() | generateBottomHalfDiagonalMovementQueen(queenIndex, 9, isWhiteQueen));
    attacks.setBitboard(attacks.getBitboard() | generateTopHalfDiagonalMovementQueen(queenIndex, 9, isWhiteQueen));
    attacks.setBitboard(attacks.getBitboard() | generateTopHalfDiagonalMovementQueen(queenIndex, 7, isWhiteQueen));

    // create mask for negative gradient diagonal
    return attacks.getBitboard();
}
