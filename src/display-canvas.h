#ifndef CHESS_DISPLAYCANVAS_H
#define CHESS_DISPLAYCANVAS_H

#include "opengl-helper.h"

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <glm/glm.hpp>

/**
 * A panel that displays the chessboard. This is the class that will be most
 * tightly linked to OpenGL.
 */
class DisplayCanvas : public wxGLCanvas
{
  public:
    /** Constructs a blank canvas, and creates all relevant buffers. */
    DisplayCanvas(wxWindow *parent);

  private:
    /** The OpenGL context for this window. */
    wxGLContext* context_;


    /** The shading program for the grid. */
    GLuint grid_program_;

    /** The VAO for the grid. */
    GLuint grid_vao_;

    /** The VBO for the grid. */
    GLuint grid_vbo_;

    // TODO use indexed drawing to do pieces


    /** The horizontal angle the board is viewed from. */
    float theta_;

    /** The vertical angle the board is viewed from. */
    float phi_;


    /** The view matrix. Takes worldspace to cameraspace. */
    glm::mat4 camera_;

    /** The projection matrix. Takes cameraspace to screenspace. */
    glm::mat4 projection_;

    /**
     * Initializes all the OpenGL specific variables and sets up global state.
     * This cannot be called until there is a valid OpenGL context! As a
     * result, this can't be called until the panel is shown.
     */
    void initializeOpenGL();

    /** Initializes the VAO and VBO for the grid. */
    void initializeGrid();

    /** Renders the board to the panel. */
    void render(wxPaintEvent& evt);

    /** Renders the grid to the panel. */
    void renderGrid();
};

#endif
