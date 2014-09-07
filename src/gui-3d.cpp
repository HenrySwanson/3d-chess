#include "gui-3d.h"

Gui3D::Gui3D() : wxFrame(NULL, wxID_ANY, wxT("3D Chess"), wxDefaultPosition, wxSize(500,500))
{
    board = new Board();
    board->setup();

    // Note: Don't have to destruct child windows
    display_canvas = new DisplayCanvas(this, board);
}

Gui3D::~Gui3D()
{
    delete board;   
}
