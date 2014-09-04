#include "display-canvas.h"

DisplayCanvas::DisplayCanvas(wxWindow *parent) : wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"), wxNullPalette)
{
    context_ = new wxGLContext(this); // TODO possibly destruct?

    Connect(GetId(), wxEVT_PAINT, wxPaintEventHandler(DisplayCanvas::render));

    // TODO generate vao and vbos
    // TODO bind vao and vbos
    // TODO buffer vbos
    // TODO configure vao attributes

    // TODO compile shaders
}

void DisplayCanvas::render(wxPaintEvent& evt)
{
    SetCurrent(*context_);
    wxPaintDC(this);

    wxSize size = GetClientSize();
    glViewport(0, 0, size.x, size.y);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO bind vao and program
    // TODO draw lines

    SwapBuffers();
}
