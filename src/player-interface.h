#ifndef CHESS_PLAYERINTERFACE_H
#define CHESS_PLAYERINTERFACE_H

#include "move.h"

class PlayerInterface
{
  public:
    /** Virtual destructor. */
    virtual ~PlayerInterface() {};

    /** Informs the player that they can now make a move. */
    virtual void notify() = 0;
};

#endif
