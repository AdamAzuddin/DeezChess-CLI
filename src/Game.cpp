#include <iostream>
#include "include/Game.h"
#include "include/Board.h"
#include <string>

using std::string;

Game::Game() : board(this)
{
    isOver = false;
    isCheck = false;
    isCheckmate = false;
    isDraw = false;
    isWhiteMove = true;
}

void Game::run()
{
    std::cout << "Starting a new game...\n";
    Board* board;
    board->initialize();

    while (!isOver)
    {
        string move;

        if (board->playerMove(isWhiteMove))
        {
            isWhiteMove = !isWhiteMove;
        }
    }

    delete board;
}