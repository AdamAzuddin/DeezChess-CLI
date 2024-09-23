#include <iostream>
#include "include/Board.h"
#include <bitset>
#include "include/Game.h"
Board::Board(Game *game) : game(game)
{
}

void Board::printBoard()
{
    std::vector<std::pair<Bitboard, char>> pieces = {
        {whitePawns, 'P'},
        {whiteKnights, 'N'},
        {whiteRooks, 'R'},
        {whiteBishops, 'B'},
        {whiteQueens, 'Q'},
        {whiteKing, 'K'},
        {blackPawns, 'p'},
        {blackKnights, 'n'},
        {blackRooks, 'r'},
        {blackBishops, 'b'},
        {blackQueens, 'q'},
        {blackKing, 'k'},

    };

    for (int rank = 7; rank >= 0; --rank)
    {
        for (int file = 0; file < 8; ++file)
        {
            bool pieceFound = false;
            int bitPosition = rank * 8 + file;
            for (const auto &[bitboardObj, symbol] : pieces)
            {
                if (bitboardObj.getBitboard() & (1ULL << bitPosition))
                {
                    std::cout << symbol << ' ';
                    pieceFound = true;
                    break;
                }
            }
            if (!pieceFound)
            {
                std::cout << "- ";
            }
        }
        std::cout << "\n";
    }
}

void Board::initialize()
{
    whitePawns.setBitboard(0x000000000000FF00ULL);
    whitePawns.setWhitePiece(true);

    whiteRooks.setBitboard(0x0000000000000081ULL);
    whiteRooks.setWhitePiece(true);

    whiteKnights.setBitboard(0x0000000000000042ULL);
    whiteKnights.setWhitePiece(true);

    whiteBishops.setBitboard(0x0000000000000024ULL);
    whiteBishops.setWhitePiece(true);

    whiteQueens.setBitboard(0x0000000000000008ULL);
    whiteQueens.setWhitePiece(true);

    whiteKing.setBitboard(0x0000000000000010ULL);
    whiteKing.setWhitePiece(true);

    blackPawns.setBitboard(0x00FF000000000000ULL);
    blackPawns.setWhitePiece(false);

    blackRooks.setBitboard(0x8100000000000000ULL);
    blackRooks.setWhitePiece(false);

    blackKnights.setBitboard(0x4200000000000000ULL);
    blackKnights.setWhitePiece(false);

    blackBishops.setBitboard(0x2400000000000000ULL);
    blackBishops.setWhitePiece(false);

    blackQueens.setBitboard(0x0800000000000000ULL);
    blackQueens.setWhitePiece(false);

    blackKing.setBitboard(0x1000000000000000ULL);
    blackKing.setWhitePiece(false);

    initializeKingAttackMasks();

    whiteControlledSquares.setBitboard(0);
    blackControlledSquares.setBitboard(0);

    whitePieces.setBitboard(
        whitePawns.getBitboard() | whiteRooks.getBitboard() | whiteKnights.getBitboard() |
        whiteBishops.getBitboard() | whiteQueens.getBitboard() | whiteKing.getBitboard());

    whitePieces.setWhitePiece(true);

    blackPieces.setBitboard(
        blackPawns.getBitboard() | blackRooks.getBitboard() | blackKnights.getBitboard() |
        blackBishops.getBitboard() | blackQueens.getBitboard() | blackKing.getBitboard());

    blackPieces.setWhitePiece(false);

    occupiedSquares.setBitboard(whitePieces.getBitboard() | blackPieces.getBitboard());

    numOfMovesWithoutCaptureOrCheck = 0;

    whiteCanShortCastle = true;
    whiteCanLongCastle = true;
    blackCanShortCastle = true;
    blackCanLongCastle = true;

    printBoard();
}

void Board::initializeKingAttackMasks()
{
    for (int square = 0; square < 64; square++)
    {
        unsigned long long kingBitboard = 1ULL << square;
        unsigned long long attacks = 0;
        attacks |= (kingBitboard << 8);                // north
        attacks |= (kingBitboard >> 8);                // south
        attacks |= (kingBitboard & ~FILE_A_MASK) >> 1; // west
        attacks |= (kingBitboard & ~FILE_H_MASK) << 1; // east
        attacks |= (kingBitboard & ~FILE_A_MASK) << 7; // north-west
        attacks |= (kingBitboard & ~FILE_H_MASK) << 9; // north-east
        attacks |= (kingBitboard & ~FILE_A_MASK) >> 9; // south-west
        attacks |= (kingBitboard & ~FILE_H_MASK) >> 7; // south-east
        kingAttackTable[square] = attacks;
    }
};


int Board::deduceCapturingPawnIndex(char fromFile, int targetIndex)
{
    int targetFiled = targetIndex % 8;
    int targetRank = targetIndex / 8;

    int originalFile = fromFile - 'a';
    int originalRank = whiteToMove ? targetRank - 1 : targetRank + 1;

    int originalIndex = originalRank * 8 + originalFile;
    return originalIndex;
}
int Board::convertCoordinateToBitboardIndex(const std::string &coordinate)
{
    char file = coordinate[0]; // 'a' to 'h'
    char rank = coordinate[1]; // '1' to '8'

    // Convert file ('a'-'h') to a number between 0 and 7
    int fileIndex = file - 'a';

    // Convert rank ('1'-'8') to a number between 0 and 7
    // Rank '8' corresponds to index 7, so we subtract '1' to align it with bitboard index.
    int rankIndex = rank - '1';

    return (rankIndex * 8) + fileIndex;
}

bool Board::playerMove(bool isWhite)
{
    whiteToMove = isWhite; // use the isWhite var from game for whiteToMove var in Board so there's no need to pass props to all the methods
    auto *myControlledSquareBeforeRemoveMyPiece = whiteToMove ? (&whiteControlledSquares) : (&blackControlledSquares);
    auto *enemyKing = whiteToMove ? (&blackKing) : (&whiteKing);

    if (isWhite)
    {
        std::cout << "White to move: ";
    }
    else
    {
        std::cout << "Black to move: ";
    }
    string currentMove;

    std::cin >> currentMove;
    std::cout << "\n";

    // check if move is valid, meaning in format of Ne4, exd5, etc

    if (Board::isValidInput(currentMove) != -1)
    {
        if (isLegalMove(currentMove))
        {
            std::cout << "Your move " << currentMove << " is legal\n";

            int originalEnemyKingIndex = enemyKing->getIndicesOfPieces()[0];
            Bitboard* movedPiece = currentMovingPiece;
            // check if its a check to the enemy
            if (myControlledSquareBeforeRemoveMyPiece->getBitboard() & enemyKing->getBitboard())
            {
                std::cout << "You are giving a check!\n";

                unsigned long long kingAttacksBeforeCapture = calculateKingAttacks(enemyKing) & ~(1ULL << targetIndex);
                unsigned long long myControlledSquareBeforeRemovePieceAtTargetIndex = myControlledSquareBeforeRemoveMyPiece->getBitboard();

                pieceToMoveIndex = originalEnemyKingIndex;

                capturePieceInBitboard(enemyKing, currentMovingPiece); // try enemy king capture our piece

                auto myControlledSquareAfterRemoveMyPiece = whiteToMove ? whiteControlledSquares : blackControlledSquares;
                
                // try moving enemy king to capture our piece
                // if when we update our controlled squares it also convers that square, then its checkmate

                // if its checkmate
                if ((kingAttacksBeforeCapture & myControlledSquareBeforeRemovePieceAtTargetIndex) == kingAttacksBeforeCapture && myControlledSquareAfterRemoveMyPiece.getBitboard() & 1ULL << targetIndex)
                {
                    std::cout << "You gave a checkmate!\n";
                    if (whiteToMove)
                        std::cout << "White won!\n";
                    else
                        std::cout << "Black won!\n";

                    game->isOver = true;
                    return true;
                }
                else
                {
                    uncapturePieceInBitboard(enemyKing, movedPiece);
                }
            }

            // check if only king is left, then draw
            if (whitePieces.getIndicesOfPieces().size() == 1 && blackPieces.getIndicesOfPieces().size() == 1){
                std::cout<<"It's a draw since only the king is left";
                game->isOver = true;
            }
            printBoard();
            return true;
        }
        else
        {
            std::cout << "Your move " << currentMove << " is not legal. Try again.\n";
            printBoard();
            return false;
        }

        return true;
    }
    else
    {
        std::cout << "Your move " << currentMove << " is not valid. Try again.\n";
        printBoard();
        return false;
    }
}
