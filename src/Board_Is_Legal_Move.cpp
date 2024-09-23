#include <iostream>
#include "include/Board.h"

bool Board::isLegalMove(string currentMove)
{
    // does it leave my king in check.
    // move the piece and update all necessary bitboards
    Bitboard *enemyControlledSquare = whiteToMove ? (&blackControlledSquares) : (&whiteControlledSquares);
    Bitboard *myKing = whiteToMove ? (&whiteKing) : (&blackKing);

    Bitboard *pieceToMove;
    Bitboard *pieceToBeCaptured;

    bool *rightToShortCastle = whiteToMove ? (&whiteCanShortCastle) : (&blackCanShortCastle);
    bool *rightToLongCastle = whiteToMove ? (&whiteCanLongCastle) : (&blackCanLongCastle);

    bool onPromotionRank = whiteToMove ? (targetIndex >= 56 && targetIndex <= 63) : (targetIndex >= 0 && targetIndex <= 7);

    if (currentMove.find("x") != string::npos)
    {

        const string coordinate = currentMove.substr(currentMove.size() - 2);
        int pieceToCaptureIndex;

        auto &enemyPawns = whiteToMove ? blackPawns : whitePawns;
        auto &enemyRooks = whiteToMove ? blackRooks : whiteRooks;
        auto &enemyKnights = whiteToMove ? blackKnights : whiteKnights;
        auto &enemyBishops = whiteToMove ? blackBishops : whiteBishops;
        auto &enemyQueens = whiteToMove ? blackQueens : whiteQueens;
        auto &enemyKing = whiteToMove ? blackKing : whiteKing;

        targetIndex = convertCoordinateToBitboardIndex(coordinate);
        std::cout << "Target index: " << targetIndex;
        unsigned long long targetMask = 1ULL << targetIndex;

        if (targetMask & enemyPawns.getBitboard())
        {
            pieceToBeCaptured = (&enemyPawns);
        }
        else if (targetMask & enemyRooks.getBitboard())
        {
            pieceToBeCaptured = (&enemyRooks);
        }
        else if (targetMask & enemyKnights.getBitboard())
        {
            pieceToBeCaptured = (&enemyKnights);
        }
        else if (targetMask & enemyBishops.getBitboard())
        {
            pieceToBeCaptured = (&enemyBishops);
        }
        else if (targetMask & enemyQueens.getBitboard())
        {
            pieceToBeCaptured = (&enemyQueens);
        }
        else if (targetMask & enemyKing.getBitboard())
        {
            std::cout << "You cannot capture a king.";
            return false;
        }
        else
        {
            std::cout << "No enemy piece found at" << coordinate << ". Try again.\n";
            return false;
        }

        switch (pieceToMoveType)
        {
        case 'P':
            pieceToMove = whiteToMove ? (&whitePawns) : (&blackPawns);

            if (onPromotionRank)
            {
                char promoteToPiece;
                Bitboard *pieceToPromoteToBitboard;
                std::cout << "Your pawn can promote. What do you want to promote it to? ";
                std::cin >> promoteToPiece;
                switch (promoteToPiece)
                {
                case 'Q':
                    pieceToPromoteToBitboard = whiteToMove ? (&whiteQueens) : (&blackQueens);
                    break;
                case 'R':
                    pieceToPromoteToBitboard = whiteToMove ? (&whiteRooks) : (&blackRooks);
                    break;
                case 'N':
                    pieceToPromoteToBitboard = whiteToMove ? (&whiteKnights) : (&blackKnights);
                    break;
                case 'B':
                    pieceToPromoteToBitboard = whiteToMove ? (&whiteBishops) : (&blackBishops);
                    break;

                default:
                    std::cout << "Invalid promotion.\n";
                    return false;
                    break;
                }
                pieceToBeCaptured->removeAtIndex(targetIndex);
                promotePawn(pieceToMove, pieceToPromoteToBitboard);
                std::cout << "\nPawn promoted successfully!";
                return true;
            }
            break;
        case 'N':
            pieceToMove = whiteToMove ? (&whiteKnights) : (&blackKnights);
            break;
        case 'B':
            pieceToMove = whiteToMove ? (&whiteBishops) : (&blackBishops);
            break;
        case 'Q':
            pieceToMove = whiteToMove ? (&whiteQueens) : (&blackQueens);
            break;
        case 'R':
            pieceToMove = whiteToMove ? (&whiteRooks) : (&blackRooks);
            if (whiteToMove)
            {
                if (pieceToMoveIndex == 7)
                    whiteCanShortCastle = false;
                else if (pieceToMoveIndex == 0)
                    whiteCanLongCastle = false;
            }
            else
            {
                if (pieceToMoveIndex == 63)
                    blackCanShortCastle = false;
                else if (pieceToMoveIndex == 56)
                    blackCanLongCastle = false;
            }
            break;
        case 'K':
            pieceToMove = whiteToMove ? (&whiteKing) : (&blackKing);
            *rightToShortCastle = false;
            *rightToLongCastle = false;
            break;
        default:
            std::cout << "Invalid piece type\n";
            return false;
        }

        capturePieceInBitboard(pieceToMove, pieceToBeCaptured);

        if (enemyControlledSquare->getBitboard() & myKing->getBitboard())
        {
            std::cout << "Illegal capture\n";
            uncapturePieceInBitboard(pieceToMove, pieceToBeCaptured);

            return false;
        }
        else
        {
            return true;
        }
    }
    else if (currentMove == "0-0")
    {
        if (whiteToMove)
        {
            if (whiteCanShortCastle && whiteKing.getBitboard() & (1ULL << 4) && whiteRooks.getBitboard() & (1ULL << 7) && !(blackControlledSquares.getBitboard() & 1ULL << 5) && !(blackControlledSquares.getBitboard() & 1ULL << 6) && !(blackControlledSquares.getBitboard() & whiteKing.getBitboard()))
            {
                std::cout << "Castling is legal\n";
                whiteKing.removeAtIndex(4);
                whiteKing.addAtIndex(6);
                whiteRooks.removeAtIndex(7);
                whiteRooks.addAtIndex(5);
                updatePiecesBitboards();
                updateWhiteControlledSquare();
                updateBlackControlledSquare();
                whiteCanShortCastle = false;
                whiteCanShortCastle = false;
            }
            else
            {
                std::cout << "Castling is illegal.\n";
                return false;
            }
        }
        else
        {
            if (blackCanShortCastle && blackKing.getBitboard() & (1ULL << 60) && blackRooks.getBitboard() & (1ULL << 63) && !(whiteControlledSquares.getBitboard() & 1ULL << 61) && !(whiteControlledSquares.getBitboard() & 1ULL << 62) && !(whiteControlledSquares.getBitboard() & blackKing.getBitboard()))
            {

                std::cout << "Castling is legal\n";
                blackKing.removeAtIndex(60);
                blackKing.addAtIndex(62);
                blackRooks.removeAtIndex(63);
                blackRooks.addAtIndex(61);
                updatePiecesBitboards();
                updateWhiteControlledSquare();
                updateBlackControlledSquare();
                blackCanShortCastle = false;
                blackCanLongCastle = false;
            }
            else
            {
                std::cout << "Castling is illegal.\n";
                return false;
            }
        }
    }

    else if (currentMove == "0-0-0")
    {
        if (whiteToMove)
        {
            if (whiteCanLongCastle && whiteKing.getBitboard() & 1ULL << 4 && whiteRooks.getBitboard() & 1ULL << 0 && !(blackControlledSquares.getBitboard() & 1ULL << 2) && !(blackControlledSquares.getBitboard() & 1ULL << 3) && !(blackControlledSquares.getBitboard() & whiteKing.getBitboard()))
            {
                std::cout << "Castling is legal\n";
                whiteKing.removeAtIndex(4);
                whiteKing.addAtIndex(2);
                whiteRooks.removeAtIndex(0);
                whiteRooks.addAtIndex(3);
                updatePiecesBitboards();
                updateWhiteControlledSquare();
                updateBlackControlledSquare();
                whiteCanShortCastle = false;
                whiteCanShortCastle = false;
            }
            else
            {
                std::cout << "Castling is illegal.\n";
                return false;
            }
        }
        else
        {
            if (blackCanLongCastle && blackKing.getBitboard() & 1ULL << 60 && blackRooks.getBitboard() & 1ULL << 56 && !(whiteControlledSquares.getBitboard() & 1ULL << 58) && !(whiteControlledSquares.getBitboard() & 1ULL << 59) && !(whiteControlledSquares.getBitboard() & blackKing.getBitboard()))
            {

                std::cout << "Castling is legal\n";
                blackKing.removeAtIndex(60);
                blackKing.addAtIndex(58);
                blackRooks.removeAtIndex(56);
                blackRooks.addAtIndex(59);
                updatePiecesBitboards();
                updateWhiteControlledSquare();
                updateBlackControlledSquare();
                blackCanShortCastle = false;
                blackCanLongCastle = false;
            }
            else
            {
                std::cout << "Castling is illegal.\n";
                return false;
            }
        }
    }

    // normal move without capture

    switch (pieceToMoveType)
    {
    case 'P':
        pieceToMove = whiteToMove ? (&whitePawns) : (&blackPawns);

        if (onPromotionRank)
        {
            char promoteToPiece;
            Bitboard *pieceToPromoteToBitboard;
            std::cout << "Your pawn can promote. What do you want to promote it to? ";
            std::cin >> promoteToPiece;
            switch (promoteToPiece)
            {
            case 'Q':
                pieceToPromoteToBitboard = whiteToMove ? (&whiteQueens) : (&blackQueens);
                break;
            case 'R':
                pieceToPromoteToBitboard = whiteToMove ? (&whiteRooks) : (&blackRooks);
                break;
            case 'N':
                pieceToPromoteToBitboard = whiteToMove ? (&whiteKnights) : (&blackKnights);
                break;
            case 'B':
                pieceToPromoteToBitboard = whiteToMove ? (&whiteBishops) : (&blackBishops);
                break;

            default:
                std::cout << "Invalid promotion.\n";
                return false;
                break;
            }

            promotePawn(pieceToMove, pieceToPromoteToBitboard);
            std::cout << "\nPawn promoted successfully!";
            return true;
        }
        break;

    case 'R':
        pieceToMove = whiteToMove ? (&whiteRooks) : (&blackRooks);
        if (whiteToMove)
        {
            std::cout << "Piece to move index: " << pieceToMoveIndex << "\n";
            if (pieceToMoveIndex == 7)
            {
                std::cout << "White can no longer short castle\n";
                whiteCanShortCastle = false;
            }
            else if (pieceToMoveIndex == 0)
                whiteCanLongCastle = false;
        }
        else
        {
            if (pieceToMoveIndex == 63)
                blackCanShortCastle = false;
            else if (pieceToMoveIndex == 56)
                blackCanLongCastle = false;
        }
        break;

    case 'N':
        pieceToMove = whiteToMove ? (&whiteKnights) : (&blackKnights);
        break;

    case 'B':
        pieceToMove = whiteToMove ? (&whiteBishops) : (&blackBishops);
        break;

    case 'Q':
        pieceToMove = whiteToMove ? (&whiteQueens) : (&blackQueens);
        break;

    case 'K':
        pieceToMove = whiteToMove ? (&whiteKing) : (&blackKing);
        rightToShortCastle = whiteToMove ? (&whiteCanShortCastle) : (&blackCanShortCastle);
        rightToLongCastle = whiteToMove ? (&whiteCanLongCastle) : (&blackCanLongCastle);
        *rightToShortCastle = false;
        *rightToLongCastle = false;
        break;

    default:
        return false;
        break;
    }

    if (currentMove != "0-0" || currentMove != "0-0-0" || !(pieceToMoveType == 'P' && onPromotionRank))
    {
        movePieceInBitboard(pieceToMove);
    }

    if (enemyControlledSquare->getBitboard() & myKing->getBitboard())
    {
        std::cout << currentMove << " is illegal because it put your king in check\n";
        unmovePieceInBitboard(pieceToMove);
        return false;
    }

    return true;
};
