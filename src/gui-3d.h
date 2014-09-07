#ifndef CHESS_GUI3D_H
#define CHESS_GUI3D_H

#include <wx/wx.h>

#include "display-canvas.h"
#include "board.h"

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

    /** The board that this GUI displays and modifies. */
    Board* board;

  private:
    /** The canvas on which the board is drawn. */
    DisplayCanvas* display_canvas;
};

#endif