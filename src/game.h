#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <stack>
#include <thread>

#include "board.h"
#include "move.h"

#include "player-interface.h"

class Game
{
  public:
    Game(PlayerInterface* white, PlayerInterface* black);
    ~Game();

    void start();
    // TODO end (which will involve creating and joining a thread)

    // TODO undo and redo
  private:
    bool turn_;
    Board board_;
    std::stack<Move> undone_moves_;

    PlayerInterface* players_ [2];

    std::thread game_thread_;

    void run();
};

#endif
