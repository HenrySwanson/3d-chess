#include "human-player.h"

HumanPlayer::HumanPlayer()
{
    ready_ = false;
}

void HumanPlayer::setMove(Move m)
{
    std::lock_guard<std::mutex> lock(mutex_);

    // If getMove hasn't yet been called, skip this
    if(ready_)
        return;

    move_ = m;
    ready_ = true;

    waiting_.notify_one();
}

Move HumanPlayer::getMove()
{
    std::unique_lock<std::mutex> lock(mutex_);

    code_ = NONE;

    // Protects against spurious wakeup
    while(!ready_)
        waiting_.wait(lock);

    ready_ = false;

    return move_;
}

void HumanPlayer::interrupt(InterruptCode code)
{
    std::lock_guard<std::mutex> lock(mutex_);

    // If we're ready, why interrupt?
    if(ready_)
        return;

    code_ = code;
    ready_ = true;

    waiting_.notify_one();
}

InterruptCode HumanPlayer::getInterruptCode() const
{
    return code_;
}
