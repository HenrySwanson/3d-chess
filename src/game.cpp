#include "game.h"

Game::Game(PlayerInterface* white, PlayerInterface* black)
{
    players_[WHITE] = white;
    players_[BLACK] = black;
}

Game::~Game()
{
    end();
}

void Game::start()
{
    turn_ = WHITE;
    board_.setup();
    interrupted_ = false;
    game_thread_ = std::thread(&Game::run, this);
}

void Game::end()
{
    // TODO somehow, this has to interrupt a player and tell them to GTFO
    // maybe have PlayerInterface require interrupt()?
    interrupted_ = true;
    game_thread_.join();
}

void Game::run()
{
    while(!interrupted_ && board_.getGameState() == IN_PROGRESS)
    {
        Move m = players_[turn_]->requestMove(turn_, board_);
        board_.makeMove(m);
        turn_ = !turn_;
    }
}
