#ifndef CHESS_DISPLAYCANVAS_H
#define CHESS_DISPLAYCANVAS_H

#include <wx/wx.h>
#include <wx/glcanvas.h>

class DisplayCanvas : public wxGLCanvas
{
  public:
    DisplayCanvas(wxWindow *parent);
  private:
    wxGLContext* context_;

    void render(wxPaintEvent& evt);
};

#endif
