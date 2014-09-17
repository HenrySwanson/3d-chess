#ifndef CHESS_PLAYERINTERFACE_H
#define CHESS_PLAYERINTERFACE_H

#include "move.h"

class PlayerInterface
{
  public:
    /** Virtual destructor. */
    virtual ~PlayerInterface() {};

    /** Returns the move that the player has decided to make. */
    virtual Move getMove() = 0;
};

#endif
