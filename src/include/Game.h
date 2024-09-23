#ifndef GAME_H
#define GAME_H
#include <iostream>
#include "Board.h"

class Game
{
public:
    Game();
    Board board;
    bool isOver;
    bool isCheck;
    bool isCheckmate;
    bool isDraw;
    bool isWhiteMove;

    void run();
    void checkDraw();
    void checkMate();
    void move(bool isWhite);
    int convertCoordinateToIndex(const std::string &coordinate);
};

#endif
