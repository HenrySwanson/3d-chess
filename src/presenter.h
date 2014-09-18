#ifndef CHESS_PRESENTER_H
#define CHESS_PRESENTER_H

#include <list>
#include <string>

#include "common.h"

#include "observer.h"
#include "subject.h"

#include "human-player.h"

#include "game.h"
#include "piece.h"

// TODO perhaps couple more tightly to the view?

/**
 * Not sure what to say about this. It doesn't correspond to anything nice and
 * "physical", but it behaves as a middle layer between the model and the view.
 * In MVP terms, it's either a Supervising Controller, or the Presenter in a
 * Passive View setup.
 */
class Presenter : public Observer, public Subject
{
  public:
    /** Constructs a presenter that is attached to the given view. */
    Presenter();

    /** Destructs the presenter. Not responsible for destructing the view. */
    ~Presenter();


    /** Returns the piece at the given location. */
    Piece getPiece(int i, int j, int k) const;

    /** Returns a list of the targets of possible moves. */
    std::list<Cell> getMoveIndicators() const;

    /** Returns the list of moves in the game's history. */
    std::list<std::string> getMoveHistory() const;

    /** 
     * Interprets a click at the given location, and performs the appropriate
     * action.
     */
    void click(int i, int j, int k);


    virtual void onNotify();

  private:
    /** The game the presentation modifies. */
    Game* game_;

    HumanPlayer* player_;


    /** The cell currently selected. */   
    int selected_cell_;

    /** The moves the selected piece can make. */
    std::list<Move> selected_moves_;


    /** Clears the selected piece and available moves. */
    void clearSelection();
};

#endif
