#ifndef CHESS_PRESENTER_H
#define CHESS_PRESENTER_H

#include <list>
#include <string>

#include "common.h"

#include "human-player.h"

#include "game.h"
#include "piece.h"

// TODO make this a controller for DisplayView. That'll get rid of a lot of
// methods that are just "parse this from model and pass it on". Part of this
// will involve getting the view to know about the model. That allows it to
// directly observe the model, so this no longer needs to be a subject/observer

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
    Presenter();

    /** Destructs the presenter. Not responsible for destructing the view. */
    ~Presenter();


    /** Returns the underlying game that this presenter handles. */
    const Game* getGame() const;


    /** Returns a list of the targets of possible moves. */
    std::list<Cell> getMoveIndicators() const;

    /** Returns the list of moves in the game's history. */
    std::list<std::string> getMoveHistory() const;

    /** 
     * Interprets a click at the given location, and performs the appropriate
     * action.
     */
    void click(int i, int j, int k);

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
