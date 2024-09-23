#include <iostream>
#include <vector>
#include "include/Board.h"


unsigned long long Board::generateRookAttackMask(int rookIndex, bool isWhiteRook)
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

    int currentIndex = rookIndex;

    unsigned long long rookAttackMask = 0;

    while (true)
    {
        currentIndex += 8;

        if (currentIndex > 63)
            break;

        northMask = 1ULL << currentIndex;
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

unsigned long long Board::generateTopHalfDiagonal(int index, int shift, bool isWhiteBishop)
{
    unsigned long long currentMask;
    int currentIndex = index;
    Bitboard result;
    auto& friendlyPieces= isWhiteBishop ? whitePieces : blackPieces;
    auto& enemyPieces = isWhiteBishop ? blackPieces : whitePieces;
    result.setBitboard(0);

    unsigned long long leftRightTopEdgeMask = FILE_A_MASK | FILE_H_MASK | RANK_8_MASK;

    currentMask = 1ULL << index;

    while (true)
    {
        currentIndex += shift;
        currentMask = 1ULL << currentIndex;
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

unsigned long long Board::generateBottomHalfDiagonal(int index, int shift, bool isWhiteBishop)
{
    unsigned long long currentMask;
    int currentIndex = index;
    Bitboard result;
    auto& friendlyPieces = isWhiteBishop ? whitePieces : blackPieces;
    auto& enemyPieces = isWhiteBishop ? blackPieces : whitePieces;
    result.setBitboard(0);

    unsigned long long leftRightBottomEdgeMask = FILE_A_MASK | FILE_H_MASK | RANK_1_MASK;

    currentMask = 1ULL << index;

    while (true)
    {

        currentIndex -= shift;

        currentMask = 1ULL << currentIndex;
        if (currentMask & friendlyPieces.getBitboard() || currentIndex < 0)
        {
            break;
        }

        if (currentMask & leftRightBottomEdgeMask || currentMask & enemyPieces.getBitboard()) // including
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

unsigned long long Board::generateBishopAttackMask(int bishopIndex, bool isWhiteBishop)
{
    Bitboard attacks;

    // create mask for positive gradient diagonal
    attacks.setBitboard(generateBottomHalfDiagonal(bishopIndex, 7, isWhiteBishop));
    attacks.setBitboard(attacks.getBitboard() | generateBottomHalfDiagonal(bishopIndex, 9, isWhiteBishop));
    attacks.setBitboard(attacks.getBitboard() | generateTopHalfDiagonal(bishopIndex, 9, isWhiteBishop));
    attacks.setBitboard(attacks.getBitboard() | generateTopHalfDiagonal(bishopIndex, 7, isWhiteBishop));

    // create mask for negative gradient diagonal
    return attacks.getBitboard();
}

unsigned long long Board::generateKnightAttackMask(int knightIndex)
{
    Bitboard attacks;
    unsigned long long knightMask = 1ULL << knightIndex;
    unsigned long long FILE_AB_MASK = FILE_A_MASK | FILE_B_MASK;
    unsigned long long FILE_GH_MASK = FILE_G_MASK | FILE_H_MASK;

    // movement of knight for horizonatally with mask over at the edge of the board

    unsigned long long attackNorthNorthWest = (knightMask & ~FILE_A_MASK) << 15;  // 2 up, 1 left
    unsigned long long attackNorthNorthEast = (knightMask & ~FILE_H_MASK) << 17;  // 2 up 1 right
    unsigned long long attackSouthSouthhEast = (knightMask & ~FILE_H_MASK) >> 15; // 2 down, 1 right
    unsigned long long attackSouthSouthhWest = (knightMask & ~FILE_A_MASK) >> 17; // 2 down, 1 left

    // movement of knight vertically with mask over at the edge of the board

    unsigned long long attackWestWestNorth = (knightMask & ~FILE_AB_MASK) << 6;  // 2 left, 1 up
    unsigned long long attackWestWestSouth = (knightMask & ~FILE_AB_MASK) >> 10; // 2 left, 1 down
    unsigned long long attackEastEastNorth = (knightMask & ~FILE_GH_MASK) << 10; // 2 right, 1 up
    unsigned long long attackEastEastSouth = (knightMask & ~FILE_GH_MASK) >> 6;  // 2 right, 1 down

    attacks.setBitboard(attackNorthNorthWest | attackNorthNorthEast | attackSouthSouthhEast | attackSouthSouthhWest | attackWestWestNorth | attackWestWestSouth | attackEastEastNorth | attackEastEastSouth);

    return attacks.getBitboard();
}
