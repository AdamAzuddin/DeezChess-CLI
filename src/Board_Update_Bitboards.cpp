#include <iostream>
#include "include/Board.h"

void Board::updatePiecesBitboards()
{
    // Combine all white pieces
    whitePieces.setBitboard(whitePawns.getBitboard() | whiteRooks.getBitboard() | whiteKnights.getBitboard() | whiteBishops.getBitboard() | whiteQueens.getBitboard() | whiteKing.getBitboard());

    // Combine all black pieces
    blackPieces.setBitboard(blackPawns.getBitboard() | blackRooks.getBitboard() | blackKnights.getBitboard() | blackBishops.getBitboard() | blackQueens.getBitboard() | blackKing.getBitboard());

    // Combine all pieces to get occupied squares
    occupiedSquares.setBitboard(whitePieces.getBitboard() | blackPieces.getBitboard());
}

void Board::updateWhiteControlledSquare()
{
    whiteControlledSquares.setBitboard(0ULL);

    // Calculate control for each white piece type and update the controlled squares

    whiteControlledSquares.setBitboard(
        whiteControlledSquares.getBitboard() | calculatePawnAttacks(&whitePawns));

    whiteControlledSquares.setBitboard(
        whiteControlledSquares.getBitboard() | calculateRookAttacks(&whiteRooks));
    whiteControlledSquares.setBitboard(
        whiteControlledSquares.getBitboard() | calculateBishopAttacks(&whiteBishops));
    whiteControlledSquares.setBitboard(
        whiteControlledSquares.getBitboard() | calculateQueenAttacks(&whiteQueens));
    whiteControlledSquares.setBitboard(
        whiteControlledSquares.getBitboard() | calculateKingAttacks(&whiteKing));
    whiteControlledSquares.setBitboard(
        whiteControlledSquares.getBitboard() | calculateKnightAttacks(&whiteKnights));
}

void Board::updateBlackControlledSquare()
{
    blackControlledSquares.setBitboard(0ULL);

    // Calculate control for each black piece type and update the controlled squares

    blackControlledSquares.setBitboard(
        blackControlledSquares.getBitboard() | calculatePawnAttacks(&blackPawns));
    blackControlledSquares.setBitboard(
        blackControlledSquares.getBitboard() | calculateRookAttacks(&blackRooks));
    blackControlledSquares.setBitboard(
        blackControlledSquares.getBitboard() | calculateBishopAttacks(&blackBishops));
    blackControlledSquares.setBitboard(
        blackControlledSquares.getBitboard() | calculateQueenAttacks(&blackQueens));
    blackControlledSquares.setBitboard(
        blackControlledSquares.getBitboard() | calculateKingAttacks(&blackKing));
    blackControlledSquares.setBitboard(
        blackControlledSquares.getBitboard() | calculateKnightAttacks(&blackKnights));
}
