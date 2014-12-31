#ifndef CHESS_AIPLAYER_H
#define CHESS_AIPLAYER_H

#include "player-interface.h"

#include "game.h"

class AiPlayer : public PlayerInterface
{
  public:
    AiPlayer(bool color, Game* game);
    ~AiPlayer();
    virtual void notify();

  private:
    bool color_;
    Game* game_;
};

#endif
