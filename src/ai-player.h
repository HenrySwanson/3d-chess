#ifndef CHESS_AIPLAYER_H
#define CHESS_AIPLAYER_H

#include "player-interface.h"

#include "game.h"

class AiPlayer : public PlayerInterface
{
  public:
    AiPlayer();
    ~AiPlayer();
    virtual Move requestMove(bool color, const Board& board);

  private:
};

#endif
