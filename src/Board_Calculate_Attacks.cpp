#include <iostream>
#include <vector>
#include "include/Board.h"

unsigned long long Board::calculatePawnAttacks(Bitboard *pawns)
{
    Bitboard pawnControlledSquare;
    pawnControlledSquare.setBitboard(0ULL); // trying to limit the squares that the enemy is controlling by not including where their own piece is blocking it
    auto& friendlyPieces = pawns->isWhitePiece() ? whitePieces : blackPieces;
    unsigned long long northEastAttackMask = 0ULL;
    unsigned long long northWestAttackMask = 0ULL;

    std::vector<int> pawnIndices = (*pawns).getIndicesOfPieces();

    for (int i = 0; i < pawnIndices.size(); i++)
    {
        int pawnIndex = pawnIndices[i];

        // Reset attack masks each time
        northEastAttackMask = 0ULL;
        northWestAttackMask = 0ULL;

        // Check if it's not on the A-file or H-file
        if (pawnIndex % 8 != 0 && (pawnIndex + 1) % 8 != 0)
        {
            // General case: not on A-file or H-file
            (*pawns).isWhitePiece() ? northEastAttackMask = 1ULL << (pawnIndex + 9) : northEastAttackMask = 1ULL << (pawnIndex - 7);
            (*pawns).isWhitePiece() ? northWestAttackMask = 1ULL << (pawnIndex + 7) : northWestAttackMask = 1ULL << (pawnIndex - 9);
        }
        else if (pawnIndex % 8 == 0)
        {
            // A-file: only northeast attacks
            (*pawns).isWhitePiece() ? northEastAttackMask = 1ULL << (pawnIndex + 9) : northEastAttackMask = 1ULL << (pawnIndex - 7);
        }
        else if ((pawnIndex + 1) % 8 == 0)
        {
            // H-file: only northwest attacks
            (*pawns).isWhitePiece() ? northWestAttackMask = 1ULL << (pawnIndex + 7) : northWestAttackMask = 1ULL << (pawnIndex - 9);
        }

        // Combine the attack masks
        pawnControlledSquare.setBitboard(pawnControlledSquare.getBitboard() | northEastAttackMask | northWestAttackMask);
    }

    // check if theres a friendly piece;
    if (pawnControlledSquare.getBitboard() & friendlyPieces.getBitboard())
    {
        pawnControlledSquare.setBitboard(pawnControlledSquare.getBitboard() & ~friendlyPieces.getBitboard());
    }

    return pawnControlledSquare.getBitboard();
}


unsigned long long Board::calculateRookAttacks(Bitboard* rooks)
{
    unsigned long long rookIndexMask;

    Bitboard rookAttackMask;
    rookAttackMask.setBitboard(0);
    std::vector<int> rookIndices = rooks->getIndicesOfPieces();

    for (int i = 0; i < rookIndices.size(); i++)
    {
        rookAttackMask.setBitboard(generateRookAttackMask(rookIndices[i], rooks->isWhitePiece()) & ~(1ULL << rookIndices[i]) | rookAttackMask.getBitboard());
    }
    rookAttackMask.setBitboard(rookAttackMask.getBitboard() & ~rooks->getBitboard());

    return rookAttackMask.getBitboard();
}

unsigned long long Board::calculateBishopAttacks(Bitboard* bishops)
{
    unsigned long long bishopBitboard = bishops->getBitboard();

    Bitboard bishopAttackMask;
    bishopAttackMask.setBitboard(0);

    std::vector<int> bishopIndices = bishops->getIndicesOfPieces();

    for (int i = 0; i < bishopIndices.size(); i++)
    {
        bishopAttackMask.setBitboard(bishopAttackMask.getBitboard() | generateBishopAttackMask(bishopIndices[i], bishops->isWhitePiece()));
    }

    return bishopAttackMask.getBitboard();
}

unsigned long long Board::calculateQueenAttacks(Bitboard* queens)
{
    unsigned long long queenBitboard = (*queens).getBitboard();
    Bitboard queenAttackMask;
    queenAttackMask.setBitboard(0);

    std::vector<int> queenIndices = (*queens).getIndicesOfPieces();

    for (int i = 0; i < queenIndices.size(); i++)
    {
        queenAttackMask.setBitboard(queenAttackMask.getBitboard() | generateRookAttackMask(queenIndices[i], queens->isWhitePiece()) | generateBishopAttackMask(queenIndices[i], queens->isWhitePiece()));
    }

    queenAttackMask.setBitboard(queenAttackMask.getBitboard() & ~(*queens).getBitboard());

    return queenAttackMask.getBitboard();
}

unsigned long long Board::calculateKingAttacks(Bitboard* king)
{

    auto& friendlyPieces = king->isWhitePiece() ? whitePieces : blackPieces;
    unsigned long long kingBitboard = king->getBitboard();
    int kingIndex;
    int index = 0;

    while (kingBitboard != 0)
    {
        if (kingBitboard & 1)
        {
            kingIndex = index;
        }
        kingBitboard >>= 1;
        ++index;
    }
    Bitboard attacks;
    attacks.setBitboard(kingAttackTable[kingIndex] & ~friendlyPieces.getBitboard());

    return attacks.getBitboard();
}

unsigned long long Board::calculateKnightAttacks(Bitboard* knights)
{
    unsigned long long knightBitboard = (*knights).getBitboard();
    unsigned long long attacks = 0ULL;
    Bitboard knightAttackMask;
    auto& friendlyPieces = knights->isWhitePiece() ? whitePieces : blackPieces;
    knightAttackMask.setBitboard(0);

    std::vector<int> knightIndices = (*knights).getIndicesOfPieces();

    for (int i = 0; i < knightIndices.size(); i++)
    {
        knightAttackMask.setBitboard(knightAttackMask.getBitboard() | generateKnightAttackMask(knightIndices[i]));
    }

    return knightAttackMask.getBitboard() & ~friendlyPieces.getBitboard(); // not return square thats occupied by friendly pieces
}
