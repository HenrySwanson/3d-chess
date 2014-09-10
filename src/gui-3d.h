#ifndef CHESS_GUI3D_H
#define CHESS_GUI3D_H

#include <wx/wx.h>

#include <string>

#include "view-interface.h"
#include "display-canvas.h"

#include "presenter.h"

/**
 * A 3D graphical interface for the game. When this object is initialized,
 * it creates a new game and board, as well as all relevant windows.
 */
class Gui3D : public wxFrame, public ViewInterface
{
  public:
    /** Standard constructor */
    Gui3D();

    /** Standard destructor */
    ~Gui3D();

    /** Tells this window and its children to check the model for updates. */
    virtual void refresh();

  private:
    /** The presentation layer this view displays. */
    Presenter* presenter_;

    /** The canvas on which the board is drawn. */
    DisplayCanvas* display_canvas_;

    wxListBox* move_history_;

    wxButton* button_undo;
    wxButton* button_redo;

    void newGame(wxCommandEvent& evt);
    void undoMove(wxCommandEvent& evt);
    void redoMove(wxCommandEvent& evt);
};

#endif
