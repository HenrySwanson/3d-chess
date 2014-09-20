#ifndef CHESS_PLAYERINTERFACE_H
#define CHESS_PLAYERINTERFACE_H

#include "move.h"

enum InterruptCode {
    NONE, END
};

class PlayerInterface
{
  public:
    /** Virtual destructor. */
    virtual ~PlayerInterface() {};

    /** Returns the move that the player has decided to make. */
    virtual Move getMove() = 0;

    /** Interrupts the player with the given (non-zero) error code. */
    virtual void interrupt(InterruptCode code) = 0;

    /** Returns a non-zero value if this player was interrupted. */
    virtual InterruptCode getInterruptCode() const = 0;
};

#endif
