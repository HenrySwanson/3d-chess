#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <stack>

#include "board.h"
#include "move.h"

#include "player-interface.h"

class Game
{
  public:
    Game(PlayerInterface* white, PlayerInterface* black);
    ~Game();

    // TODO undo and redo
  private:
    // TODO should these be atomic?
    bool turn_;
    Board board_;
    std::stack<Move> undone_moves_;

    PlayerInterface* players_ [2];
};

#endif
