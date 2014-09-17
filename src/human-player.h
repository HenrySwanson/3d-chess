#ifndef CHESS_HUMANPLAYER_H
#define CHESS_HUMANPLAYER_H

#include <condition_variable>
#include <mutex>

#include "player-interface.h"
#include "move.h"

// TODO this is not thread-safe! what happens when you call getMove, and before turn is changed, another setMove is called?

class HumanPlayer : public PlayerInterface
{
  public:
    HumanPlayer();

    /** Returns the move that the player has decided to make. */
    virtual Move getMove();

    void setMove(Move m);

  private:
    bool ready_;
    Move move_;

    std::mutex mutex_;
    std::condition_variable cond_var_;
};

#endif
