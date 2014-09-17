#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <stack>

#include <atomic>
#include <thread>

#include "board.h"
#include "move.h"

#include "player-interface.h"

// TODO move to board?
enum GameState {
    IN_PROGRESS, CHECKMATE_WHITE, CHECKMATE_BLACK, STALEMATE_WHITE,
    STALEMATE_BLACK
};

// TODO maybe make the view observe this? otherwise how can it know when the ai makes a move?
class Game
{
  public:
    Game(PlayerInterface* white, PlayerInterface* black);
    ~Game();

    bool getTurn() const;
    const Board& getBoard() const;

    void begin();
    void end();

    // TODO undo and redo
  private:
    Board board_;
    bool turn_;
    std::stack<Move> undid_moves_;

    PlayerInterface* players_ [2];

    std::atomic<bool> running_;
    std::thread game_thread_;

    void run();
};

#endif
