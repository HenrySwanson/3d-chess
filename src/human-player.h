#ifndef CHESS_HUMANPLAYER_H
#define CHESS_HUMANPLAYER_H

#include <mutex>
#include <condition_variable>

#include "player-interface.h"
#include "view-interface.h"
#include "board.h"
#include "move.h"

// TODO should I use futures to handle this, instead of condvars?
class HumanPlayer : public PlayerInterface
{
  public:
    HumanPlayer(ViewInterface* view);
    ~HumanPlayer();

    bool isReady() const;
    bool whoseTurn() const;
    Board getBoard() const;

    void submitMove(const Move& m);
    Move requestMove(bool color, const Board& board);

  private:
    ViewInterface* view_;

    bool turn_;
    Board board_;

    bool ready_;
    std::mutex mutex_;
    std::condition_variable cv_;

    Move submitted_move_;
};

#endif
