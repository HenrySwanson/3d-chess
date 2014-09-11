#include "display-canvas.h"

#include <glm/gtx/color_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

using glm::mat4;
using glm::vec3;
using glm::vec4;

/** Pi */
static const float PI = 3.1415926535f;

/** Distance of the camera from the board center */
static const float EYE_RAD = 15;

/** Attributes to set up the OpenGL context */
static int OPEN_GL_ATTRIBS [] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

/**
 * Indexed by PieceType. Indicates the offset in the VBO that the piece's model
 * starts at.
 */
static const int PIECE_MODEL_OFFSET[16] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0
};

/**
 * Indexed by PieceType. Indicates the number of triangles that the piece's
 * model uses.
 */
static const int PIECE_MODEL_LENGTH[16] = {
    0, 0, 6, 6,
    6, 6, 6, 6,
    6, 6, 6,
    6, 6, 6,
    6, 6
};

DisplayCanvas::DisplayCanvas(wxWindow *parent, Presenter* presenter) : wxGLCanvas(parent, wxID_ANY, OPEN_GL_ATTRIBS, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"), wxNullPalette)
{
    presenter_ = presenter;

    context_ = new wxGLContext(this);
    opengl_initialized = false;

    theta_ = -PI/2;
    phi_ = PI/2;

    SetMinSize(wxSize(500, 500));

    // Connect events to their handlers
    Connect(GetId(), wxEVT_LEFT_DOWN, wxMouseEventHandler(DisplayCanvas::handleMouseDown));
    Connect(GetId(), wxEVT_LEFT_UP, wxMouseEventHandler(DisplayCanvas::handleMouseUp));
    Connect(GetId(), wxEVT_MOTION, wxMouseEventHandler(DisplayCanvas::handleMouseDrag));
    Connect(GetId(), wxEVT_PAINT, wxPaintEventHandler(DisplayCanvas::paint));
}

DisplayCanvas::~DisplayCanvas()
{
    glDeleteProgram(grid_object_.program);
    glDeleteProgram(piece_object_.program);

    glDeleteVertexArrays(1, &grid_object_.vao);
    glDeleteVertexArrays(1, &piece_object_.vao);

    glDeleteBuffers(1, &grid_object_.vbo);
    glDeleteBuffers(1, &piece_object_.vbo);

    // It's possible that deleting this obviates the need for the lines above.
    // But I don't know.
    delete context_;
}

void DisplayCanvas::initializeOpenGL()
{
    SetCurrent(*context_);

    // Initialize GLEW
    GLenum err = glewInit();
    if(err != GLEW_OK)
        std::cout << "GLEW failed to initialize!" << std::endl;

    // Enable depth test and backface culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    glEnable(GL_CULL_FACE);

    // Create programs
    grid_object_.program = makeProgram("resources/grid.vertexshader", "resources/grid.fragmentshader");
    piece_object_.program = makeProgram("resources/piece.vertexshader", "resources/piece.fragmentshader");
    indicator_object_.program = makeProgram("resources/indicator.vertexshader", "resources/indicator.fragmentshader");

    // Create VAOs and VBOs
    initializeGrid();
    initializePieces();
    initializeIndicators();

    opengl_initialized = true;
}

void DisplayCanvas::initializeGrid()
{
    // Make and bind the VBO and VAO
    glGenBuffers(1, &grid_object_.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, grid_object_.vbo);
    glGenVertexArrays(1, &grid_object_.vao);
    glBindVertexArray(grid_object_.vao);

    // Write vertices for the lines of the grid
    GLfloat vertexData [3 * 2 * 81 * 3];
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            int index = (i * 9 + j) * 3 * 2 * 3;
            GLfloat a = (GLfloat) i - 4;
            GLfloat b = (GLfloat) j - 4;
            GLfloat c = (GLfloat) 4;
            GLfloat lines [18] = {
                a, b,-c,  a, b, c,
                a,-c, b,  a, c, b,
               -c, a, b,  c, a, b
            };
            memcpy(vertexData + index, lines, 18 * sizeof(GLfloat));
        }
    }

    // Load that array into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    
    // Configure the "vert" variable of the shader program
    GLuint in_vert = glGetAttribLocation(grid_object_.program, "vert");
    glEnableVertexAttribArray(in_vert);
    glVertexAttribPointer(in_vert, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DisplayCanvas::initializePieces()
{
    // Make and bind the VBO and VAO
    glGenBuffers(1, &piece_object_.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, piece_object_.vbo);
    glGenVertexArrays(1, &piece_object_.vao);
    glBindVertexArray(piece_object_.vao);

    // TODO replace with piece-by-piece loading
    std::vector<float> faces = loadObjFile("resources/pyramid.obj");

    // Load that array into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * faces.size(), &faces[0], GL_STATIC_DRAW);

    // Configure the "vert" variable of the shader program
    GLuint in_vert = glGetAttribLocation(piece_object_.program, "vert");
    glEnableVertexAttribArray(in_vert);
    glVertexAttribPointer(in_vert, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DisplayCanvas::initializeIndicators()
{
    // Make and bind the VBO and VAO
    glGenBuffers(1, &indicator_object_.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, indicator_object_.vbo);
    glGenVertexArrays(1, &indicator_object_.vao);
    glBindVertexArray(indicator_object_.vao);

    // Fill an array with data
    GLfloat vertexData [3 * 3 * 8] = {
        0.5, 0.5, 0.8,    0.8, 0.5, 0.5,    0.5, 0.8, 0.5,
        0.5, 0.5, 0.8,    0.5, 0.8, 0.5,    0.2, 0.5, 0.5,
        0.5, 0.5, 0.8,    0.2, 0.5, 0.5,    0.5, 0.2, 0.5,
        0.5, 0.5, 0.8,    0.5, 0.2, 0.5,    0.8, 0.5, 0.5,
        0.5, 0.5, 0.2,    0.8, 0.5, 0.5,    0.5, 0.2, 0.5,
        0.5, 0.5, 0.2,    0.5, 0.2, 0.5,    0.2, 0.5, 0.5,
        0.5, 0.5, 0.2,    0.2, 0.5, 0.5,    0.5, 0.8, 0.5,
        0.5, 0.5, 0.2,    0.5, 0.8, 0.5,    0.8, 0.5, 0.5
    };

    // Load that array into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Configure the "vert" variable of the shader program
    GLuint in_vert = glGetAttribLocation(indicator_object_.program, "vert");
    glEnableVertexAttribArray(in_vert);
    glVertexAttribPointer(in_vert, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DisplayCanvas::handleMouseDown(wxMouseEvent& evt)
{
    old_mouse_pos_ = evt.GetPosition();
    has_dragged_ = false;
}

void DisplayCanvas::handleMouseUp(wxMouseEvent& evt)
{
    if(has_dragged_)
        return;

    // Render just the pieces and indicators (to the back buffer, to avoid flicker)
    prerender();
    renderPieces();
    renderIndicators();

    vec3 world_coords;
    if(unprojectClick(evt.GetPosition(), world_coords))
    {
        // Shift from worldspace into "chess-space". The +0.02 is to prevent
        // the base of the piece from sticking into the square below it.
        vec3 loc = glm::floor(world_coords + vec3(4, 4, 4.02));
        presenter_->click(loc.x, loc.y, loc.z);
    }
}

void DisplayCanvas::handleMouseDrag(wxMouseEvent& evt)
{
    if(!evt.LeftIsDown())
        return;

    has_dragged_ = true;

    wxPoint delta = evt.GetPosition() - old_mouse_pos_;
    old_mouse_pos_ += delta;

    // The camera is inverted, as makes sense for mice. The 200 is arbitrary.
    theta_ -= (delta.x * PI / 200);
    phi_ -= (delta.y * PI / 200);

    // Keep theta from drifting too far from 0
    theta_ = glm::mod(theta_, 2 * PI);
    // Keep the user from flipping things upside-down
    phi_ = glm::clamp(phi_, 0.001f, PI - 0.001f);

    Refresh();
}

bool DisplayCanvas::unprojectClick(wxPoint pt, vec3& world_coords)
{
    // Get screen size and viewport
    wxSize size = GetClientSize();
    vec4 viewport(0, 0, size.x, size.y);

    // Gets the location of the mouse click in window space. wxWidgets sets the
    // origin in the upper left, but OpenGL wants it in the lower left,
    GLint winX = pt.x;
    GLint winY = size.y - pt.y - 1; // ... hence the thing here

    // Gets the z-value from the depth-buffer
    GLfloat winZ;
    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    // Did we really click anything?
    if(winZ == 1)
        return false;

    // Do the unprojection
    vec3 window (winX, winY, winZ);
    world_coords = glm::unProject(window, view_, proj_, viewport);

    return true;
}

void DisplayCanvas::updateMatrices()
{
    // View
    vec3 eye (EYE_RAD * sin(phi_) * cos(theta_),
              EYE_RAD * sin(phi_) * sin(theta_),
              EYE_RAD * cos(phi_));

    view_ = glm::lookAt(eye, vec3(0,0,0), vec3(0,0,1));

    // Projection
    wxSize size = GetClientSize();
    proj_ = glm::perspective<float>(50.0, (float) size.x / size.y, 8.0, 22.0);
}

void DisplayCanvas::paint(wxPaintEvent& evt)
{
    // If we haven't already, initialize OpenGL
    if(!opengl_initialized)
        initializeOpenGL();

    // Become up-to-date
    updateMatrices();

    // Clear screen, then draw to it
    prerender();
    renderGrid();
    renderPieces();
    renderIndicators();

    // Display our brand-new picture!
    SwapBuffers();
}

void DisplayCanvas::prerender()
{
    // Prepare for drawing by setting context
    SetCurrent(*context_);
    wxPaintDC(this);

    // Sets the viewport to match the screen size
    wxSize size = GetClientSize();
    glViewport(0, 0, size.x, size.y);

    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DisplayCanvas::renderGrid()
{
    // Bind the program and VAO
    glUseProgram(grid_object_.program);
    glBindVertexArray(grid_object_.vao);

    // Bind uniform variables
    mat4 vp = proj_ * view_;
    GLuint vp_loc = glGetUniformLocation(grid_object_.program, "VP");
    glUniformMatrix4fv(vp_loc, 1, false, glm::value_ptr(vp));

    // Draw
    glDrawArrays(GL_LINES, 0, 2 * 81 * 3);

    // Unbind everything
    glBindVertexArray(0);
    glUseProgram(0);
}

// TODO render actual pieces
void DisplayCanvas::renderPieces()
{
    // Bind the program and VAO
    GLuint program = piece_object_.program;
    glUseProgram(program);
    glBindVertexArray(piece_object_.vao);

    // Set up view-projection matrix
    mat4 vp = proj_ * view_;
    GLuint vp_loc = glGetUniformLocation(program, "VP");
    glUniformMatrix4fv(vp_loc, 1, false, glm::value_ptr(vp));

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                // Compute model matrix
                vec3 corner = vec3(i - 4, j - 4, k - 4);
                mat4 model = glm::translate(mat4(), corner);

                // Compute hue
                PieceType pt = presenter_->getPiece(i,j,k).type();
                if(pt == NIL || pt == BORDER)
                    continue;
                int tmp = (pt == W_PAWN) ? B_PAWN : pt; // Makes pawns match
                float hue = tmp * (360.0f / 13);
                vec3 color = glm::rgbColor(vec3(hue, 1, 1));

                // Set uniform variables
                GLuint m_loc = glGetUniformLocation(program, "M");
                GLuint color_loc = glGetUniformLocation(program, "color");
                glUniformMatrix4fv(m_loc, 1, false, glm::value_ptr(model));
                glUniform3f(color_loc, color.r, color.g, color.b);

                // Find the piece model in the VBO and draw!
                int offset = PIECE_MODEL_OFFSET[pt];
                int length = PIECE_MODEL_LENGTH[pt];
                glDrawArrays(GL_TRIANGLES, offset * 3, length * 3);
            }
        }
    }

    // Unbind everything
    glBindVertexArray(0);
    glUseProgram(0);
}

// TODO rounder!
void DisplayCanvas::renderIndicators()
{
    // Bind the program and VAO
    GLuint program = indicator_object_.program;
    glUseProgram(program);
    glBindVertexArray(indicator_object_.vao);

    // Set up view-projection matrix
    mat4 vp = proj_ * view_;
    GLuint vp_loc = glGetUniformLocation(program, "VP");
    glUniformMatrix4fv(vp_loc, 1, false, glm::value_ptr(vp));

    std::list<Cell> indicators = presenter_->getMoveIndicators();

    std::list<Cell>::const_iterator it;
    for(it = indicators.begin(); it != indicators.end(); it++)
    {
        // Compute model matrix
        vec3 corner = vec3(it->x - 4, it->y - 4, it->z - 4);
        mat4 model = glm::translate(mat4(), corner);

        // Set uniform variables
        GLuint m_loc = glGetUniformLocation(program, "M");
        glUniformMatrix4fv(m_loc, 1, false, glm::value_ptr(model));

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, 3 * 8);
    }

    // Unbind everything
    glBindVertexArray(0);
    glUseProgram(0);
}
