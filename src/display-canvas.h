#ifndef CHESS_DISPLAYCANVAS_H
#define CHESS_DISPLAYCANVAS_H

// GLEW must be declared before GL
#include "opengl-helper.h"

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <glm/glm.hpp>

#include "presenter.h"

/**
 * A panel that displays the chessboard. This is the class that will be most
 * tightly linked to OpenGL.
 */
class DisplayCanvas : public wxGLCanvas
{
  public:
    /** Constructs a blank canvas, and creates all relevant buffers. */
    DisplayCanvas(wxWindow *parent, Presenter* presenter);

    /**
     * Destructs the object and destroys all associated OpenGL resources.
     */
    ~DisplayCanvas();

  private:
    struct RenderObject // TODO name better; also, make one for each piece (sharing references)
    {
        GLuint program, vao, vbo;
    };

    /** The presentation layer this panel displays. */
    Presenter* presenter_;


    /** The OpenGL context for this window. */
    wxGLContext* context_;

    /** Whether or not context_ has been initialized. */
    bool opengl_initialized;


    /** The render object for the grid. */
    RenderObject grid_object_;

    /** The render object for the pieces. */
    RenderObject piece_object_;

    /** The render object for the move indicators. */
    RenderObject indicator_object_;

    /** The offsets, in vertices, that each piece model starts at. */
    int piece_model_offset_ [17];


    /** The horizontal angle the board is viewed from. */
    float theta_;

    /** The vertical angle the board is viewed from. */
    float phi_;

    /** The view matrix most recently used to render the screen. */
    glm::mat4 view_;

    /** The projection matrix most recently used to render the screen. */
    glm::mat4 proj_;


    /** Used to store the last known location of the mouse. */
    wxPoint old_mouse_pos_;

    /** If the mouse is down and has moved, this is true. */
    bool has_dragged_;


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

    /** Initializes the VAOs and VBO for the move indicators. */
    void initializeIndicators();


    /** Sets the old mouse position. */
    void handleMouseDown(wxMouseEvent& evt);

    /** Checks if a click has occurred, and handles the resulting action. */
    void handleMouseUp(wxMouseEvent& evt);

    /** Handles mouse dragging and the resulting camera rotation. */
    void handleMouseDrag(wxMouseEvent& evt);


    /**
     * Given a click location, examines the back buffer to determine where in
     * worldspace this click occurred. Returns false and does not modify the
     * vector if no object was clicked.
     */
    bool unprojectClick(wxPoint pt, glm::vec3& world_coords);

    /** Recomputes the view and projection matrices. */
    void updateMatrices();


    /** Displays the board on the panel. */
    void paint(wxPaintEvent& evt);

    /** Resizes the viewport and clears the back buffer. */
    void prerender();

    /** Renders the grid to the back buffer. */
    void renderGrid();

    /** Renders the pieces to the back buffer. */
    void renderPieces();

    /** Renders the move indicators to the back buffer. */
    void renderIndicators();
};

#endif
