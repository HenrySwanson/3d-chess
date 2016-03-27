#include "human-player.h"

HumanPlayer::HumanPlayer(ViewInterface* view)
{
    view_ = view;
    // Doesn't really matter what the values are; they should be set by
    // requestMove before submitMove is called.
    submitted_ = true;
    interrupted_ = true;
}

#include <iostream>
HumanPlayer::~HumanPlayer()
{
    // Interrupt any thread waiting on requestMove before continuing.
    interrupt();
    // TODO stall so that we don't die before requestMove completes...?
}

bool HumanPlayer::waitingForMove() const
{
    return !(submitted_ || interrupted_);
}

bool HumanPlayer::whoseTurn() const
{
    return turn_;
}

Board HumanPlayer::getBoard() const
{
    return board_;
}

void HumanPlayer::submitMove(const Move& m)
{
    std::lock_guard<std::mutex> lock(mutex_);

    submitted_move_ = m;
    submitted_ = true;
    cv_.notify_one();
}

Move HumanPlayer::requestMove(bool color, const Board& board)
{
    std::unique_lock<std::mutex> lock(mutex_);

    submitted_ = false;
    interrupted_ = false;
    turn_ = color;
    board_ = board;

    // Refresh the view
    view_->refresh();

    // Protects against spurious wakeup
    while(!submitted_ && !interrupted_)
    {
        cv_.wait(lock);
        if(interrupted_)
            submitted_move_ = Move();
    }

    return submitted_move_;
}

void HumanPlayer::interrupt()
{
    interrupted_ = true;
    cv_.notify_one();
}
