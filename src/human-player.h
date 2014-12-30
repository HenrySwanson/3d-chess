#ifndef CHESS_HUMANPLAYER_H
#define CHESS_HUMANPLAYER_H

#include "player-interface.h"

class HumanPlayer : public PlayerInterface
{
  public:
    HumanPlayer();

    virtual void notify();
};

#endif
