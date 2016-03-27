#ifndef CHESS_HUMANPLAYER_H
#define CHESS_HUMANPLAYER_H

#include <condition_variable>
#include <mutex>

#include "board.h"
#include "move.h"
#include "player-interface.h"
#include "view-interface.h"

// TODO should the view handle be moved into Game? that way, two AIs can
// play against each other and someone can watch it happen

// TODO should I use futures to handle this, instead of condvars?

/**
 * Represents a human-controlled player. This can be applied to several
 * situations, but as a general, this is useful when the moves are provided by
 * another thread's event loop. This also has a handle to a view, and is
 * responsible for refreshing it.
 */
class HumanPlayer : public PlayerInterface
{
  public:
    /** Constructs a human player who uses the given view. */
    HumanPlayer(ViewInterface* view);

    /** Default destructor. Doesn't destruct the view. */
    ~HumanPlayer();


    /** Returns true if there is a thread waiting on requestMove. */
    bool waitingForMove() const;

    /**
     * Returns the color passed in by requestMove. Undefined if no thread is
     * waiting on it.
     */
    bool whoseTurn() const;

    /**
     * Returns the board state passed in by requestMove. Undefined if no thread
     * is waiting on it.
     */
    Board getBoard() const;


    /**
     * If a thread is waiting on requestMove, releases it, and passes it the
     * move parameter.
     */
    void submitMove(const Move& m);

    /**
     * When called, the thread will block until submitMove is called. The 
     * parameter to submitMove will be returned from this method.
     */
    virtual Move requestMove(bool color, const Board& board);

    /** Unblocks a thread waiting on requestMove. */
    virtual void interrupt();

  private:
    /** Used to hold requestMove's parameters. */
    bool turn_;
    Board board_;
    Move submitted_move_;


    /** The view that the human player observes. */
    ViewInterface* view_;


    /** True when submitMove() has been called. */
    bool submitted_;

    /** True when interrupt() has been called. */
    bool interrupted_;

    /** The condition for the requesting thread to wait on. */
    std::condition_variable cv_;

    /** The mutex used by the condition variable. */
    std::mutex mutex_;
};

#endif
