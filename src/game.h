#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <stack>

#include "board.h"
#include "move.h"

#include "player-interface.h"

class Game
{
  public:
    Game();
    ~Game();

    void setPlayer(bool color, PlayerInterface* player);

    bool getTurn() const;

    /** Returns a copy of the current board. */
    Board getBoard() const;

    void submitMove(const Move& m);

    // TODO undo and redo
  private:
    // TODO should these be atomic?
    Board board_;
    bool turn_;
    std::stack<Move> undone_moves_;

    PlayerInterface* players_ [2];
};

#endif
