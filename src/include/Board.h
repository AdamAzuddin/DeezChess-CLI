#ifndef BOARD_H
#define BOARD_H
#include "Bitboard.h"

using std::string;


class Game;

class Board
{
public:
    Board(Game* game);
    Game* game;
    Bitboard whitePawns, whiteRooks, whiteKnights, whiteBishops, whiteQueens, whiteKing;
    Bitboard blackPawns, blackRooks, blackKnights, blackBishops, blackQueens, blackKing;
    Bitboard whitePieces, blackPieces, occupiedSquares, whiteControlledSquares, blackControlledSquares;

    Bitboard *currentMovingPiece;

    unsigned long long FILE_A_MASK = 0x0101010101010101ULL;
    unsigned long long FILE_B_MASK = 0x0202020202020202ULL;
    unsigned long long FILE_C_MASK = 0x0404040404040404ULL;
    unsigned long long FILE_D_MASK = 0x0808080808080808ULL;
    unsigned long long FILE_E_MASK = 0x1010101010101010ULL;
    unsigned long long FILE_F_MASK = 0x2020202020202020ULL;
    unsigned long long FILE_G_MASK = 0x4040404040404040ULL;
    unsigned long long FILE_H_MASK = 0x8080808080808080ULL;

    unsigned long long RANK_1_MASK = 0x00000000000000FFULL;
    unsigned long long RANK_2_MASK = 0x000000000000FF00ULL;
    unsigned long long RANK_3_MASK = 0x0000000000FF0000ULL;
    unsigned long long RANK_4_MASK = 0x00000000FF000000ULL;
    unsigned long long RANK_5_MASK = 0x000000FF00000000ULL;
    unsigned long long RANK_6_MASK = 0x0000FF0000000000ULL;
    unsigned long long RANK_7_MASK = 0x00FF000000000000ULL;
    unsigned long long RANK_8_MASK = 0xFF00000000000000ULL;

    unsigned long long boardEdgeMask = FILE_A_MASK | RANK_8_MASK | FILE_H_MASK | RANK_1_MASK;

    unsigned long long kingAttackTable[64];

    bool whiteToMove;
    bool whiteCanShortCastle;
    bool whiteCanLongCastle;
    bool blackCanShortCastle;
    bool blackCanLongCastle;

    int whitePieceCount;
    int blackPieceCount;
    int pieceToMoveIndex;
    int targetIndex;

    char pieceToMoveType;
    float numOfMovesWithoutCaptureOrCheck; // add by 0.5 each time white or black move

    void initialize();
    void initializeKingAttackMasks();
    Bitboard getPieces(int pieceNum, bool isWhite);
    Bitboard setPieces(int pieceNum, bool isWhite);

    unsigned long long calculatePawnAttacks(Bitboard *pawns);
    unsigned long long calculateRookAttacks(Bitboard *rooks);
    unsigned long long calculateBishopAttacks(Bitboard *bishops);
    unsigned long long calculateKnightAttacks(Bitboard *knights);
    unsigned long long calculateQueenAttacks(Bitboard *queens);
    unsigned long long calculateKingAttacks(Bitboard *king);

    unsigned long long generateRookAttackMask(int rookIndex, bool isWhiteRook);
    unsigned long long generateBishopAttackMask(int bishopIndex, bool isWhiteBishop);
    unsigned long long generateTopHalfDiagonal(int index, int shift, bool isWhiteBishop);
    unsigned long long generateBottomHalfDiagonal(int index, int shift, bool isWhiteBishop);
    unsigned long long generateKnightAttackMask(int knightIndex);

    unsigned long long generateBishopMovementMask(int bishopIndex, bool isWhiteBishop);
    unsigned long long generateTopHalfDiagonalMovement(int index, int shift, bool isWhiteBishop);
    unsigned long long generateBottomHalfDiagonalMovement(int index, int shift, bool isWhiteBishop);

    unsigned long long generateRookMovementMask(int rookIndex, bool isWhiteRook);

    unsigned long long generateQueenMovementMask(int bishopIndex, bool isWhiteQueen);
    unsigned long long generateTopHalfDiagonalMovementQueen(int index, int shift, bool isWhiteQueen);
    unsigned long long generateRookMovementMaskQueen(int rookIndex, bool isWhiteQueen);
    unsigned long long generateBottomHalfDiagonalMovementQueen(int index, int shift, bool isWhiteQueen);

    int convertCoordinateToBitboardIndex(const std::string &coordinate);
    void updatePiecesBitboards();
    void updateWhiteControlledSquare();
    void updateBlackControlledSquare();
    void printBoard();

    void unmovePieceInBitboard(Bitboard *piece);
    void movePieceInBitboard(Bitboard *piece);

    void promotePawn(Bitboard *pawns, Bitboard *piecesPromotedTo);

    void capturePieceInBitboard(Bitboard *capturingPiece, Bitboard *capturedPiece);
    void uncapturePieceInBitboard(Bitboard *capturingPiece, Bitboard *capturedPiece);

    bool playerMove(bool isWhite);
    int isValidInput(string move);
    bool isValidPawnInput(string move);
    bool isValidPieceInput(string move);
    bool isValidCaptureInput(string move);
    bool isValidCoordinateInput(string coordinate);
    bool isLegalMove(string move);

    int deduceCapturingPawnIndex(char fromFile, int targetIndex);

    int possiblePawnIndex(int targetIndex);
    std::vector<int> possibleKnightIndices(int targetIndex);
    std::vector<int> possibleBishopIndices(int targetIndex);
    std::vector<int> possibleQueenIndices(int targetIndex);
    std::vector<int> possibleRookIndices(int targetIndex);
};

#endif