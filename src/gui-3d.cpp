#include "gui-3d.h"

Gui3D::Gui3D() : wxFrame(NULL, wxID_ANY, wxT("3D Chess"), wxDefaultPosition, wxDefaultSize)
{
    display_canvas = new DisplayCanvas(this);

    Center();
}

Gui3D::~Gui3D()
{
    
}
