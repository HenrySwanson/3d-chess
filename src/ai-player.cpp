#include "ai-player.h"

#include <list>

#include "board.h"
#include "move.h"

#include <iostream>

using std::list;

AiPlayer::AiPlayer(bool color, Game* game)
{
   color_ = color;
   game_ = game;
}

AiPlayer::~AiPlayer()
{

}

void AiPlayer::notify()
{
    Board b = game_->getBoard();
    list<Move> moves = b.generatePseudoLegalMoves(color_);

    list<Move>::const_iterator it;
    for(it = moves.begin(); it != moves.end(); it++)
    {
        if(b.isLegalMove(*it))
        {
            game_->submitMove(*it);
            return;
        }
    }

    // TODO how do I cleanly end the game?
    std::cout << "Checkmate" << std::endl;
}
