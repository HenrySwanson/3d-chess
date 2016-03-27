#include "game.h"

#include <iostream>

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
    players_[0]->interrupt();
    players_[1]->interrupt();
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

    // TODO this is a placeholder, replace with something more interesting,
    // possibly somewhere else
    std::cout << "Game ended in: ";
    switch(board_.getGameState())
    {
      case IN_PROGRESS:
        std::cout << "IN_PROGRESS";
        break;
      case CHECKMATE_WHITE:
        std::cout << "CHECKMATE_WHITE";
        break;
      case CHECKMATE_BLACK:
        std::cout << "CHECKMATE_BLACK";
        break;
      case STALEMATE_WHITE:
        std::cout << "STALEMATE_WHITE";
        break;
      case STALEMATE_BLACK:
        std::cout << "STALEMATE_BLACK";
        break;
      default:
        std::cout << "???";
    }
    std::cout << std::endl;
}
