#include "ai-player.h"

#include <list>

#include "board.h"
#include "move.h"

#include <iostream>

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
