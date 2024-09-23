#include <iostream>
#include "include/Board.h"

void Board::movePieceInBitboard(Bitboard* piece)
{
    piece->removeAtIndex(pieceToMoveIndex);
    piece->addAtIndex(targetIndex);

    updatePiecesBitboards();
    updateWhiteControlledSquare();
    updateBlackControlledSquare();

    currentMovingPiece = piece;
}

void Board::unmovePieceInBitboard(Bitboard* piece)
{
    piece->removeAtIndex(targetIndex);
    piece->addAtIndex(pieceToMoveIndex);

    updatePiecesBitboards();
    updateWhiteControlledSquare();
    updateBlackControlledSquare();
}

void Board::capturePieceInBitboard(Bitboard* capturingPiece, Bitboard* capturedPiece)
{
    capturingPiece->removeAtIndex(pieceToMoveIndex);
    capturedPiece->removeAtIndex(targetIndex);
    capturingPiece->addAtIndex(targetIndex);
    updatePiecesBitboards();
    updateWhiteControlledSquare();
    updateBlackControlledSquare();
    
    currentMovingPiece = capturingPiece;
}

void Board::uncapturePieceInBitboard(Bitboard* capturingPiece, Bitboard* capturedPiece)
{
    capturingPiece->removeAtIndex(targetIndex);
    capturedPiece->addAtIndex(targetIndex);
    capturingPiece->addAtIndex(pieceToMoveIndex);
    updatePiecesBitboards();
    updateWhiteControlledSquare();
    updateBlackControlledSquare();
}


void Board::promotePawn(Bitboard* pawns, Bitboard* piecesToPromotedTo){
    pawns->removeAtIndex(pieceToMoveIndex);
    piecesToPromotedTo->addAtIndex(targetIndex);

    updatePiecesBitboards();
    updateWhiteControlledSquare();
    updateBlackControlledSquare();

}