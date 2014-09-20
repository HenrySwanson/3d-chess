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

    void setMove(Move m);


    /** Returns the move that the player has decided to make. */
    virtual Move getMove();

    /** Interrupts the player with the given (non-zero) error code. */
    virtual void interrupt(InterruptCode code);

    /** Returns a non-zero value if this player was interrupted. */
    virtual InterruptCode getInterruptCode() const;

  private:
    bool ready_;
    Move move_;
    InterruptCode code_;

    std::mutex mutex_;
    std::condition_variable waiting_;
};

#endif
