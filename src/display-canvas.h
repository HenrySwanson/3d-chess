#ifndef CHESS_DISPLAYCANVAS_H
#define CHESS_DISPLAYCANVAS_H

#include "opengl-helper.h"

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <glm/glm.hpp>

class DisplayCanvas : public wxGLCanvas
{
  public:
    DisplayCanvas(wxWindow *parent);
  private:
    wxGLContext* context_;

    GLuint program_;
    GLuint grid_vao_;

    glm::mat4 camera_;
    glm::mat4 projection_;
    void initializeOpenGL();

    void render(wxPaintEvent& evt);
};

#endif
