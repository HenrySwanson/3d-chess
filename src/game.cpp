#include "game.h"

Game::Game()
{
    turn_ = WHITE;
    board_.setup();
}

void Game::setPlayer(bool color, PlayerInterface* player)
{
    players_[color] = player;
}

Game::~Game()
{

}

bool Game::getTurn() const
{
    return turn_;
}

// TODO like this is even remotely thread-safe. fix it, asshole
Board Game::getBoard() const
{
    return board_;
}

void Game::submitMove(const Move& m)
{
    // Make move and switch turns
    board_.makeMove(m);
    turn_ = !turn_;

    // Notify the next player
    players_[turn_]->notify();
}
