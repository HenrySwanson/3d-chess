#include "human-player.h"

HumanPlayer::HumanPlayer()
{
    ready_ = false;
}

Move HumanPlayer::getMove()
{
    std::unique_lock<std::mutex> lock(mutex_);

    // Protects against spurious wakeup
    while(!ready_)
        cond_var_.wait(lock);

    ready_ = false;

    return move_;
}

void HumanPlayer::setMove(Move m)
{
    // If getMove hasn't yet been called, skip this
    if(ready_) return;

    std::lock_guard<std::mutex> lock(mutex_);

    move_ = m;
    ready_ = true;

    cond_var_.notify_one();
}
