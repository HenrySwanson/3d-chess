#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <stack>
#include <thread>

#include "board.h"
#include "move.h"
#include "player-interface.h"

/**
 * Manages the entirety of a chess game, including players, board state, and
 * undone moves. It contains a thread which is used to poll the two players for
 * moves. This is separate from the main thread, and useful when one doesn't
 * want to tie up the GUI thread.
 */
class Game
{
  public:
    /** Creates a game with the two given players, but doesn't start it yet. */
    Game(PlayerInterface* white, PlayerInterface* black);

    /** Destructs the game, but not the players. */
    ~Game();


    /**
     * Begins the game. This will start a new thread, which will poll the
     * players for actions until interrupted.
     */
    void start();

    /**
     * Ends the game. This interrupts the current player's move-making, and
     * tells the game thread to leave the run-loop.
     */
    void end();

    // TODO undo and redo

  private:
    /**
     * Alternately queries each player until interrupted, starting with white.
     * The entry point for game_thread_.
     */
    void run();


    /** The thread started by start(). Currently an infinite loop. */
    std::thread game_thread_;

    /** Stores the two players in an array. */
    PlayerInterface* players_ [2];

    /** Which player's turn it currently is. */
    bool turn_;

    /** The current state of the board. */
    Board board_;

    /** If another thread has called for the game to end. */
    bool interrupted_;

    /**
     * When a move is undone, it's pushed onto this stack. This allows redoing
     * moves. Making a move clears it.
     */
    std::stack<Move> undone_moves_;
};

#endif
