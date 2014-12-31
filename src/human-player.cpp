#include "human-player.h"

HumanPlayer::HumanPlayer(ViewInterface* view)
{
    view_ = view;
    ready_ = false;
}

HumanPlayer::~HumanPlayer()
{

}

bool HumanPlayer::isReady() const
{
    return ready_;
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
    ready_ = false;
    cv_.notify_all();
}

Move HumanPlayer::requestMove(bool color, const Board& board)
{
    std::unique_lock<std::mutex> lock(mutex_);

    view_->refresh();

    ready_ = true;
    // TODO standardize turn/color
    turn_ = color;
    board_ = board;

    // Protects against spurious wakeup
    while(ready_)
        cv_.wait(lock);

    return submitted_move_;
}
