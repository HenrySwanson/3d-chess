#include "game.h"

Game::Game(PlayerInterface* white, PlayerInterface* black)
{
    players_[WHITE] = white;
    players_[BLACK] = black;
}

Game::~Game()
{
    // TODO should probably kill the other thread here?
}

void Game::start()
{
    turn_ = WHITE;
    board_.setup();
    game_thread_ = std::thread(&Game::run, this);
}

void Game::run()
{
    while(true) // TODO end somehow?
    {
        Move m = players_[turn_]->requestMove(turn_, board_);
        board_.makeMove(m);
        turn_ = !turn_;
    }
}
