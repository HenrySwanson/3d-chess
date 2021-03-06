#ifndef CHESS_GUI3D_H
#define CHESS_GUI3D_H

#include <wx/wx.h>

#include <string>

#include "ai-player.h"
#include "display-canvas.h"

/**
 * A 3D graphical interface for the game. When this object is initialized,
 * it creates a new game and board, as well as all relevant windows.
 */
class Gui3D : public wxFrame
{
  public:
    /** Standard constructor */
    Gui3D();

    /** Standard destructor */
    ~Gui3D();

  private:
    /** Triggers when the new game button is clicked. */
    void newGame(wxCommandEvent& evt);

    /** Triggers when the undo move button is clicked. */
    void undoMove(wxCommandEvent& evt);

    /** Triggers when the redo move button is clicked. */
    void redoMove(wxCommandEvent& evt);

    /** Closes the window. */
    void onClose(wxCloseEvent& evt);


    /** Responds to the game ending with the appropriate box. */
    void reactGameOver(GameState state);


    /** An AI player. */
    AiPlayer* ai_;

    /** The game being played. */
    Game* game_;


    /** The canvas on which the board is drawn. */
    DisplayCanvas* display_canvas_;

    /** The list box displaying the move history. */
    wxListBox* move_history_;

    /** The button to undo moves. */
    wxButton* button_undo;

    /** The button to redo moves. */
    wxButton* button_redo;
};

#endif
