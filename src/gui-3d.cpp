#include "gui-3d.h"

Gui3D::Gui3D() : wxFrame(NULL, wxID_ANY, wxT("3D Chess"), wxDefaultPosition, wxSize(500,500))
{
    presenter_ = new Presenter(this);

    // Note: Don't have to destruct child windows
    display_canvas = new DisplayCanvas(this, presenter_);
}

Gui3D::~Gui3D()
{
    delete presenter_;
}

void Gui3D::refresh()
{
    Refresh();
}
