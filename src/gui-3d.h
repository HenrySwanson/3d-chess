#ifndef CHESS_GUI3D_H
#define CHESS_GUI3D_H

#include <wx/wx.h>

#include "display-canvas.h"
#include "board.h"

class Gui3D : public wxFrame
{
  public:
    Gui3D();
    ~Gui3D();
    Board board;
  private:
    DisplayCanvas* display_canvas;
};

#endif
