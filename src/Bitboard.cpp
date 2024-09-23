#include <iostream>
#include "include/Bitboard.h"
#include <vector>
#include <bitset>

Bitboard::Bitboard() {

};

void Bitboard::setWhitePiece(bool isWhitePiece)
{
    isWhite = isWhitePiece;
}

bool Bitboard::isWhitePiece()
{
    return isWhite;
}

std::vector<int> Bitboard::getIndicesOfPieces()
{
    std::vector<int> indices;

    unsigned long long num = bitboard;

    int index = 0;
    while (num != 0)
    {
        if (num & 1)
        {
            indices.push_back(index);
        }
        num >>= 1;
        ++index;
    }

    return indices;
}

void Bitboard::setBitboard(unsigned long long num)
{
    bitboard = num;
}

void Bitboard::addAtIndex(int index)
{
    unsigned long long targetMoveMask = 1ULL << index;
    bitboard = bitboard | targetMoveMask;
}

void Bitboard::removeAtIndex(int index)
{
    unsigned long long pieceToMoveMask = 1ULL << index;
    bitboard = bitboard & ~pieceToMoveMask;
}

void Bitboard::printBinary()
{
    std::bitset<64> binary(bitboard);
    std::cout << "Bitboard in binary: " << binary << std::endl;
}

unsigned long long Bitboard::getBitboard() const
{
    return bitboard;
}

void Bitboard::printBitboardInRowsAndColumns()
{
    // We assume that the bitboard is a 64-bit unsigned long long
    for (int rank = 7; rank >= 0; --rank)
    { // Starting from rank 8 (top of the board)
        for (int file = 0; file < 8; ++file)
        { // Going from A to H
            // Each square on the board is represented by a bit in the bitboard
            // (rank * 8 + file) gives us the index for that square (0 to 63)
            int squareIndex = rank * 8 + file;
            // Check if the corresponding bit is set in the bitboard
            if (bitboard & (1ULL << squareIndex))
            {
                std::cout << "1 "; // Occupied square
            }
            else
            {
                std::cout << "0 "; // Empty square
            }
        }
        std::cout << std::endl; // Newline after each rank (row)
    }
}