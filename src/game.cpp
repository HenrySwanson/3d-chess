#include "game.h"

Game::Game(PlayerInterface* white, PlayerInterface* black)
{
    turn_ = WHITE;
    board_.setup();
    players_[WHITE] = white;
    players_[BLACK] = black;
}

Game::~Game()
{

}
