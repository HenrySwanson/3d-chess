#include "game.h"

Game::Game()
{
    turn_ = WHITE;
    board_.setup();
}

Game::Game(PlayerInterface* white, PlayerInterface* black)
{
    turn_ = WHITE;
    players_[0] = white;
    players_[1] = black;

    board_.setup();
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
    // TODO implement
}
