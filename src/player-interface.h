#ifndef CHESS_PLAYERINTERFACE_H
#define CHESS_PLAYERINTERFACE_H

#include "board.h"
#include "move.h"

/**
 * Provides an interface for a chess player. The player does not need to know
 * about the Game object, but instead will be passed the board state each time.
 */
class PlayerInterface
{
  public:
    /** Virtual destructor. */
    virtual ~PlayerInterface() {};

    /**
     * Asks the player to provide an appropriate move, given the board state
     * and team color.
     */
    virtual Move requestMove(bool color, const Board& board) = 0;
};

#endif
