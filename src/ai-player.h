#ifndef CHESS_AIPLAYER_H
#define CHESS_AIPLAYER_H

#include "player-interface.h"

#include "game.h"

// TODO make a class comment once your AI isn't terrible
class AiPlayer : public PlayerInterface
{
  public:
    /** Constructs an AI player. */
    AiPlayer();

    /** Default destructor. */
    ~AiPlayer();

    /**
     * Given a board state, picks the first move it can find for the given
     * color.
     */
    virtual Move requestMove(bool color, const Board& board);

  private:
};

#endif
