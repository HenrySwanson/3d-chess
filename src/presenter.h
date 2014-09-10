#ifndef CHESS_PRESENTATION_H
#define CHESS_PRESENTATION_H

#include <list>
#include <stack>
#include <string>

#include "view-interface.h"
#include "board.h"
#include "move.h"
#include "piece.h"

enum GameState {
    IN_PROGRESS, CHECKMATE_WHITE, CHECKMATE_BLACK, STALEMATE_WHITE,
    STALEMATE_BLACK
};

struct Cell
{
    int x, y, z;
};

inline bool operator==(Cell a, Cell b)
{
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

/**
 * Not sure what to say about this. It doesn't correspond to anything nice and
 * "physical", but it behaves as a middle layer between the model and the view.
 * In MVP terms, it's either a Supervising Controller, or the Presenter in a
 * Passive View setup.
 */
class Presenter
{
  public:
    /** Constructs a presenter that is attached to the given view. */
    Presenter(ViewInterface* view);

    /** Destructs the presenter. Not responsible for destructing the view. */
    ~Presenter();


    /** Returns the piece at the given location. */
    Piece getPiece(int i, int j, int k) const;

    /** Returns a list of the targets of possible moves. */
    std::list<Cell> getMoveIndicators() const;

    /** Returns the list of moves in the game's history. */
    std::list<std::string> getMoveHistory() const;

    /** Returns the state of the game. */
    GameState getGameState() const;

    /** Returns true if there are any moves in the game's history. */
    bool canUndo() const;

    /** Returns true if an undo has just been performed. */
    bool canRedo() const;


    /** 
     * Interprets a click at the given location, and performs the appropriate
     * action.
     */
    void click(int i, int j, int k);

    /** Resets the game to its initial state. */
    void newGame();

    /** Undoes the most recent move. */
    void undoMove();

    /** Redoes the most recently undone move. */
    void redoMove();

  private:
    /** The model the presentation modifies. */
    Board* model_;

    /** The view that displays this presentation. */
    ViewInterface* view_;


    /** The team whose turn it is. */
    bool turn_;

    /** The cell currently selected. */   
    int selected_cell_;

    /** The moves the selected piece can make. */
    std::list<Move> selected_moves_;

    /** A stack containing all moves that were undone. */
    std::stack<Move> undid_moves_;


    /** Switches whose turn it is, and clears the selected cell. */
    void nextTurn();
};

#endif
