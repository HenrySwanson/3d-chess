#ifndef CHESS_DISPLAYCANVAS_H
#define CHESS_DISPLAYCANVAS_H

// GLEW must be declared before GL
#include "opengl-helper.h"

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <glm/glm.hpp>

#include "board.h"

/**
 * A panel that displays the chessboard. This is the class that will be most
 * tightly linked to OpenGL.
 */
class DisplayCanvas : public wxGLCanvas
{
  public:
    /** Constructs a blank canvas, and creates all relevant buffers. */
    DisplayCanvas(wxWindow *parent, Board* board);

  private:
    /** The board that this panel displays and modifies. */
    Board* board_;


    /** The OpenGL context for this window. */
    wxGLContext* context_;


    /** The shading program for the grid. */
    GLuint grid_program_;

    /** The VAO for the grid. */
    GLuint grid_vao_;

    /** The VBO for the grid. */
    GLuint grid_vbo_;


    /** The shading program for the pieces. */
    GLuint piece_program_;

    /** The VAO for the pieces. */
    GLuint piece_vao_;

    /** The VBO for the grid. */
    GLuint piece_vbo_;


    /** The horizontal angle the board is viewed from. */
    float theta_;

    /** The vertical angle the board is viewed from. */
    float phi_;

    /** Used to store the last known location of the mouse. */
    wxPoint old_mouse_pos_;

    /** Sets the old mouse position. */
    void handleMouseDown(wxMouseEvent& evt);

    /** Handles mouse dragging and the resulting camera rotation. */
    void handleMouseDrag(wxMouseEvent& evt);


    /**
     * Initializes all the OpenGL specific variables and sets up global state.
     * This cannot be called until there is a valid OpenGL context! As a
     * result, this can't be called until the panel is shown.
     */
    void initializeOpenGL();

    /** Initializes the VAO and VBO for the grid. */
    void initializeGrid();

    /** Initializes the VAOs and VBO for the pieces. */
    void initializePieces();


    /** Renders the board to the panel. */
    void render(wxPaintEvent& evt);

    /** Renders the grid to the panel. */
    void renderGrid(glm::mat4 vp);

    /** Renders the pieces to the panel. */
    void renderPieces(glm::mat4 vp);
};

#endif
