#include <iostream>
#include "include/Board.h"

bool Board::isValidCoordinateInput(string move)
{
    static char file = move[0];
    static char rank = move[1];

    return file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8';
}

bool Board::isValidCaptureInput(string currentMove)
{
    const string coordinate = currentMove.substr(currentMove.size() - 2); // get last two char
    std::cout << "Coordinate: " << coordinate << "\n";
    targetIndex = convertCoordinateToBitboardIndex(coordinate);
    auto &enemyPieces = whiteToMove ? blackPieces : whitePieces;

    if (currentMove[0] == 'N' || currentMove[0] == 'B' || currentMove[0] == 'R' || currentMove[0] == 'Q' || currentMove[0] == 'K' || (currentMove[0] >= 'a' && currentMove[0] <= 'h'))
    {
        // check if enemy piece is on the square
        if (enemyPieces.getBitboard() & 1ULL << targetIndex)
        {
            if (currentMove[0] >= 'a' && currentMove[0] <= 'h')
            {
                pieceToMoveType = 'P';
                pieceToMoveIndex = deduceCapturingPawnIndex(currentMove[0], targetIndex);
            }
            else
            {
                pieceToMoveType = currentMove[0];
            }
            return isValidCoordinateInput(coordinate);
        }
        else
        {
            std::cout << "No enemy piece found at " << coordinate << ". Try again.\n";
            return false;
        }
    }

    else
    {
        std::cout << "Piece type is invalid";
        return false;
    }
}
bool Board::isValidPieceInput(string currentMove)
{
    std::cout << "Piece type: " << currentMove[0] << "\n";
    const string coordinate = currentMove.substr(currentMove.size() - 2); // get last two char
    std::cout << "Coordinate: " << coordinate << "\n";

    if (currentMove[0] == 'N' || currentMove[0] == 'B' || currentMove[0] == 'R' || currentMove[0] == 'Q' || currentMove[0] == 'K')
    {
        std::cout << "Valid piece to move type: " << currentMove[0] << "\n";
        return isValidCoordinateInput(coordinate);
    }

    else
    {
        std::cout << "Piece type is invalid";
        return false;
    }
}

bool Board::isValidPawnInput(string move)
{
    return isValidCoordinateInput(move);
}


int Board::isValidInput(string currentMove)
{
    if (currentMove.size() == 2)
    {
        if (Board::isValidPawnInput(currentMove))
        {
            targetIndex = convertCoordinateToBitboardIndex(currentMove);
            int possiblePawnIndex = Board::possiblePawnIndex(targetIndex);
            unsigned long long possiblePawnIndexMask = 1ULL << possiblePawnIndex;
            auto &pawns = whiteToMove ? whitePawns : blackPawns;
            if (pawns.getBitboard() & possiblePawnIndexMask)
            {
                std::cout << "A pawn can move to " << currentMove << "\n";
                pieceToMoveIndex = possiblePawnIndex;
                pieceToMoveType = 'P';
                return possiblePawnIndex;
            }
            else
            {
                std::cout << "No pawn can move to " << currentMove << " from pawn index " << possiblePawnIndex << "\n";
                return -1;
            }
        }
        else
        {
            std::cout << "Invalid pawn move to " << currentMove << "\n";
            return -1;
        }
    }
    else if (currentMove.find("x") != string::npos) // npos is returned if not found
    {
        if (Board::isValidCaptureInput(currentMove))
        {
            std::vector<int> possibleIndices;
            string coordinate = currentMove.substr(currentMove.size() - 2);

            auto &pawns = whiteToMove ? whitePawns : blackPawns;
            if (pieceToMoveType != 'P')
                pieceToMoveType = currentMove[0];

            std::cout << "Piece to move type for capture: " << pieceToMoveType << "\n";

            switch (pieceToMoveType)
            {
            case 'P':
                pieceToMoveIndex = deduceCapturingPawnIndex(currentMove[0], targetIndex);
                if (pawns.getBitboard() & 1ULL << pieceToMoveIndex)
                {
                    std::cout << "Your pawn capture is valid\n";
                    return pieceToMoveIndex;
                }
                else
                {
                    std::cout << "Invalid input.No pawn exist that can capture\n";
                    return -1;
                }
                break;

            case 'N':
                possibleIndices = possibleKnightIndices(targetIndex);
                if (possibleIndices.size() > 1)
                {
                    std::cout << "Multiple knights could move to " << coordinate << ". Please specify which one.\n";
                    return -1;
                }
                else if (possibleIndices.size() == 1)
                {
                    std::cout << "You can move a knight to " << coordinate << "\n";
                    pieceToMoveIndex = possibleIndices[0];
                    return possibleIndices[0];
                }

                else
                {
                    std::cout << "No knight found that can move to " << coordinate << "\n";
                    return -1;
                }
                break;

            case 'B':
                possibleIndices = possibleBishopIndices(targetIndex);
                if (possibleIndices.size() > 1)
                {
                    std::cout << "Multiple bishops could move to " << coordinate << ". Please specify which one.\n";
                    return -1;
                }
                else if (possibleIndices.size() == 1)
                {
                    std::cout << "You can move a bishop to " << coordinate << "\n";
                    pieceToMoveIndex = possibleIndices[0];
                    return possibleIndices[0];
                }
                else
                {
                    std::cout << "No bishop found that can move to " << coordinate << "\n";
                    return -1;
                }

            case 'Q':
                possibleIndices = possibleQueenIndices(targetIndex);
                if (possibleIndices.size() > 1)
                {
                    std::cout << "Multiple queens could move to " << coordinate << ". Please specify which one.\n";
                    return -1;
                }
                else if (possibleIndices.size() == 1)
                {
                    std::cout << "You can move a queen to " << coordinate << "\n";
                    pieceToMoveIndex = possibleIndices[0];
                    return possibleIndices[0];
                }
                else
                {
                    std::cout << "No queen found that can move to " << coordinate << "\n";
                    return -1;
                }

            case 'R':
                possibleIndices = possibleRookIndices(targetIndex);
                if (possibleIndices.size() > 1)
                {
                    std::cout << "Multiple rooks could move to " << coordinate << ". Please specify which one.\n";
                    return -1;
                }
                else if (possibleIndices.size() == 1)
                {
                    std::cout << "You can move a queen to " << coordinate << "\n";
                    pieceToMoveIndex = possibleIndices[0];
                    return possibleIndices[0];
                }
                else
                {
                    std::cout << "No queen found that can move to " << coordinate << "\n";
                    return -1;
                }

            case 'K':
                unsigned long long kingTargetIndex = kingAttackTable[targetIndex];
                auto &myKing = whiteToMove ? whiteKing : blackKing;

                if (kingTargetIndex & myKing.getBitboard())
                {
                    pieceToMoveIndex = myKing.getIndicesOfPieces()[0]; // get the only result since its not possible to have more than one king
                    std::cout << "Current king index " << pieceToMoveIndex << "\n";
                    return pieceToMoveIndex;
                }
                else
                {
                    std::cout << "No king can move to " << coordinate << "\n";
                    return -1;
                }
                break;
            }

            return 0;
            // check if valid piece capture
        }
    }
    else if (currentMove == "0-0") // short castling
    {
        bool *canShortCastle = whiteToMove ? (&whiteCanShortCastle) : (&blackCanShortCastle);

        if (!canShortCastle)
        { // if player has lose rights to castle
            std::cout << "You have already castled or moved your king or rook.\n";
            return -1;
        }
        else
        {
            return 0;
        }
    }
    else if (currentMove == "0-0-0")
    {

        bool *canLongCastle = whiteToMove ? (&whiteCanLongCastle) : (&blackCanLongCastle);
        if (!canLongCastle)
        {
            std::cout << "You have already castled or moved your king or rook.\n";
            return -1;
        }
        else
        {
            return 0;
        }
    }
    else if (currentMove.size() == 3 && currentMove.find("+") == string::npos)
    { // piece move without check from pawn, e.g: e4+ is not here
        std::cout << "Move: " << currentMove << "\n";

        pieceToMoveType = currentMove[0];
        if (isValidPieceInput(currentMove))
        {
            std::cout << "Valid move of " << currentMove << "\n";
            std::cout << "Piece to move type" << pieceToMoveType << "\n";
            std::vector<int> possibleIndices;
            string coordinate = currentMove.substr(currentMove.size() - 2);
            targetIndex = convertCoordinateToBitboardIndex(coordinate);
            switch (pieceToMoveType)
            {
            case 'N':
                possibleIndices = possibleKnightIndices(targetIndex);
                if (possibleIndices.size() > 1)
                {
                    std::cout << "Multiple knights could move to " << coordinate << ". Please specify which one.\n";
                    return -1;
                }
                else if (possibleIndices.size() == 1)
                {
                    std::cout << "You can move a knight to " << coordinate << "\n";
                    pieceToMoveIndex = possibleIndices[0];
                    return possibleIndices[0];
                }

                else
                {
                    std::cout << "No knight found that can move to " << coordinate << "\n";
                    return -1;
                }
                break;
            case 'B':
                possibleIndices = possibleBishopIndices(targetIndex);
                if (possibleIndices.size() > 1)
                {
                    std::cout << "Multiple bishops could move to " << coordinate << ". Please specify which one.\n";
                    return -1;
                }
                else if (possibleIndices.size() == 1)
                {
                    std::cout << "You can move a bishop to " << coordinate << "\n";
                    pieceToMoveIndex = possibleIndices[0];
                    return possibleIndices[0];
                }
                else
                {
                    std::cout << "No bishop found that can move to " << coordinate << "\n";
                    return -1;
                }

            case 'Q':
                possibleIndices = possibleQueenIndices(targetIndex);
                if (possibleIndices.size() > 1)
                {
                    std::cout << "Multiple queens could move to " << coordinate << ". Please specify which one.\n";
                    return -1;
                }
                else if (possibleIndices.size() == 1)
                {
                    std::cout << "You can move a queen to " << coordinate << "\n";
                    pieceToMoveIndex = possibleIndices[0];
                    return possibleIndices[0];
                }
                else
                {
                    std::cout << "No queen found that can move to " << coordinate << "\n";
                    return -1;
                }

            case 'R':
                possibleIndices = possibleRookIndices(targetIndex);
                if (possibleIndices.size() > 1)
                {
                    std::cout << "Multiple rooks could move to " << coordinate << ". Please specify which one.\n";
                    return -1;
                }
                else if (possibleIndices.size() == 1)
                {
                    std::cout << "You can move a queen to " << coordinate << "\n";
                    pieceToMoveIndex = possibleIndices[0];
                    return possibleIndices[0];
                }
                else
                {
                    std::cout << "No queen found that can move to " << coordinate << "\n";
                    return -1;
                }

            case 'K':
                unsigned long long kingTargetIndex = kingAttackTable[targetIndex];
                auto &myKing = whiteToMove ? whiteKing : blackKing;

                if (kingTargetIndex & myKing.getBitboard())
                {
                    pieceToMoveIndex = myKing.getIndicesOfPieces()[0]; // get the only result since its not possible to have more than one king
                    std::cout << "Current king index " << pieceToMoveIndex << "\n";
                    return pieceToMoveIndex;
                }
                else
                {
                    std::cout << "No king can move to " << coordinate << "\n";
                    return -1;
                }
            }

            return 0;
        }
        else
        {
            std::cout << "Invalid piece move to " << currentMove << "\n";
            return -1;
        }
    }
    std::cout << "Invalid move format\n";
    return -1;
}

