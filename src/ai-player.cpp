#include "ai-player.h"

#include <iostream>
#include <list>

using std::list;

AiPlayer::AiPlayer()
{
}

AiPlayer::~AiPlayer()
{
}

Move AiPlayer::requestMove(bool color, const Board& board)
{
    list<Move> moves = board.generatePseudoLegalMoves(color);

    list<Move>::const_iterator it;
    for(it = moves.begin(); it != moves.end(); it++)
    {
        if(board.isLegalMove(*it))
        {
            return *it;
        }
    }

    // TODO how do I cleanly end the game?
    std::cout << "Checkmate" << std::endl;
    return Move();
}

void AiPlayer::interrupt()
{
    // TODO this should do something once the AI gets slower...
    return;
}
