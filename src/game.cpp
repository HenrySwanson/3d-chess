#include "game.h"

Game::Game(PlayerInterface* white, PlayerInterface* black)
{
    turn_ = WHITE;
    players_[0] = white;
    players_[1] = black;

    running_ = false;
}

Game::~Game()
{
    end();
}

bool Game::getTurn() const
{
    return turn_;
}

const Board& Game::getBoard() const
{
    return board_;
}

// TODO make sure it's not already running
void Game::begin()
{
    running_ = true;
    game_thread_ = std::thread(&Game::run, this);
}

void Game::end()
{
    // Interrupt the current player and waits for the thread to die.
    players_[turn_]->interrupt(END);
    game_thread_.join();
}

void Game::run()
{
    board_.setup();

    while(running_ && board_.getGameState() == IN_PROGRESS)
    {
        Move m = players_[turn_]->getMove();
        InterruptCode code = players_[turn_]->getInterruptCode();

        if(code == END)
            break;

        board_.makeMove(m);
        turn_ = !turn_;
        notifyAll();
    }
}
