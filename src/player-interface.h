#ifndef CHESS_PLAYERINTERFACE_H
#define CHESS_PLAYERINTERFACE_H

#include "board.h"
#include "move.h"

class PlayerInterface
{
  public:
    /** Virtual destructor. */
    virtual ~PlayerInterface() {};

    virtual Move requestMove(bool color, const Board& board) = 0;
};

#endif
