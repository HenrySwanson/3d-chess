#include "display-canvas.h"

#include <glm/gtx/color_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

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
 * Indexed by PieceType. Indicates the number of vertices that the piece's
 * model uses.
 */
static const int PIECE_MODEL_LENGTH[16] = {
    0, 0, 18, 18,
    18, 18, 18, 18,
    18, 18, 18,
    18, 18, 18,
    18, 18
};

DisplayCanvas::DisplayCanvas(wxWindow *parent, Board* board) : wxGLCanvas(parent, wxID_ANY, OPEN_GL_ATTRIBS, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"), wxNullPalette)
{
    board_ = board;

    context_ = new wxGLContext(this); // TODO possibly destruct?

    theta_ = 0;
    phi_ = PI/2;

    // Connect events to their handlers
    Connect(GetId(), wxEVT_LEFT_DOWN, wxMouseEventHandler(DisplayCanvas::handleMouseDown));
    Connect(GetId(), wxEVT_LEFT_UP, wxMouseEventHandler(DisplayCanvas::handleMouseUp));
    Connect(GetId(), wxEVT_MOTION, wxMouseEventHandler(DisplayCanvas::handleMouseDrag));
    Connect(GetId(), wxEVT_PAINT, wxPaintEventHandler(DisplayCanvas::render));
}

void DisplayCanvas::handleMouseDown(wxMouseEvent& evt)
{
    old_mouse_pos_ = evt.GetPosition();
    has_dragged_ = false;
}

// TODO avoid clicking the gridlines
void DisplayCanvas::handleMouseUp(wxMouseEvent& evt)
{
    if(has_dragged_)
        return;

    // Get screen size and viewport
    wxSize size = GetClientSize();
    vec4 viewport(0, 0, size.x, size.y);

    // Gets the location of the mouse click. wxWidgets sets the origin in the
    // upper left, but OpenGL wants it in the lower left,
    GLint winX = evt.GetX();
    GLint winY = size.y - evt.GetY() - 1; // ... hence the thing here

    // Gets the z-value from the depth-buffer
    GLfloat depth;
    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

    // Did we really click anything?
    if(depth == 1)
        return;

    // Construct vector in screen space
    vec3 window (winX, winY, depth);

    // Do the unprojection
    vec3 loc = glm::unProject(window, view_, proj_, viewport);

    // Shift from worldspace into "chess-space".
    loc += vec3(4, 4, 4.01); // The +0.01 is to prevent the base of the piece
                             // from sticking into the square below it.
    loc = glm::floor(loc);

    std::cout << "Cube: " << loc.x << ", " << loc.y << ", " << loc.z << std::endl;
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

void DisplayCanvas::initializeOpenGL()
{
    // Initialize GLEW
    GLenum err = glewInit();
    if(err != GLEW_OK)
        std::cout << "GLEW failed to initialize!" << std::endl;

    // Enable depth test and backface culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    glEnable(GL_CULL_FACE);

    // Create programs
    grid_program_ = makeProgram("shaders/grid.vertexshader", "shaders/grid.fragmentshader");
    piece_program_ = makeProgram("shaders/piece.vertexshader", "shaders/piece.fragmentshader");

    // Create VAOs and VBOs
    initializeGrid();
    initializePieces();
}

void DisplayCanvas::initializeGrid()
{
    // Make and bind the VAO and VBO
    glGenBuffers(1, &grid_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, grid_vbo_);
    glGenVertexArrays(1, &grid_vao_);
    glBindVertexArray(grid_vao_);

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
    GLuint in_vert = glGetAttribLocation(grid_program_, "vert");
    glEnableVertexAttribArray(in_vert);
    glVertexAttribPointer(in_vert, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DisplayCanvas::initializePieces()
{
    // Make and bind the VAO and VBO
    glGenBuffers(1, &piece_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, piece_vbo_);
    glGenVertexArrays(1, &piece_vao_);
    glBindVertexArray(piece_vao_);

    // Fill an array with data
    GLfloat vertexData [3 * 3 * 6] = {
        0.2, 0.2, 0.0,    0.8, 0.2, 0.0,    0.5, 0.5, 0.7,
        0.8, 0.2, 0.0,    0.8, 0.8, 0.0,    0.5, 0.5, 0.7,
        0.8, 0.8, 0.0,    0.2, 0.8, 0.0,    0.5, 0.5, 0.7,
        0.2, 0.8, 0.0,    0.2, 0.2, 0.0,    0.5, 0.5, 0.7,
        0.2, 0.2, 0.0,    0.2, 0.8, 0.0,    0.8, 0.2, 0.0,
        0.8, 0.8, 0.0,    0.8, 0.2, 0.0,    0.2, 0.8, 0.0,
    };

    // Load that array into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Configure the "vert" variable of the shader program
    GLuint in_vert = glGetAttribLocation(grid_program_, "vert");
    glEnableVertexAttribArray(in_vert);
    glVertexAttribPointer(in_vert, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DisplayCanvas::render(wxPaintEvent& evt)
{
    // Get screen size for later
    wxSize size = GetClientSize();
    float screen_x = size.x;
    float screen_y = size.y;

    // Prepare for drawing by setting context
    SetCurrent(*context_);
    wxPaintDC(this);

    // If we haven't already, initialize OpenGL
    static bool initialized = false;
    if(!initialized)
    {
        initializeOpenGL();
        initialized = true;
    }

    // Sets the viewport to match the screen size
    glViewport(0, 0, screen_x, screen_y);

    // Locates the center of the camera, ...
    vec3 eye (EYE_RAD * sin(phi_) * cos(theta_),
              EYE_RAD * sin(phi_) * sin(theta_),
              EYE_RAD * cos(phi_));

    // and sets up the appropriate matrices.
    view_ = glm::lookAt(eye, vec3(0,0,0), vec3(0,0,1));
    proj_ = glm::perspective<float>(50.0, screen_x / screen_y, 8.0, 20.0);

    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 vp = proj_ * view_;

    renderGrid(vp);
    renderPieces(vp);

    // Display our brand-new picture!
    SwapBuffers();
}

void DisplayCanvas::renderGrid(mat4 vp)
{
    glUseProgram(grid_program_);

    // Bind uniform variables
    GLuint vp_loc = glGetUniformLocation(grid_program_, "VP");
    glUniformMatrix4fv(vp_loc, 1, false, glm::value_ptr(vp));

    // Bind the VAO and draw
    glBindVertexArray(grid_vao_);
    glDrawArrays(GL_LINES, 0, 2 * 81 * 3);

    // Unbind everything
    glBindVertexArray(0);
    glUseProgram(0);
}

// TODO render actual pieces
void DisplayCanvas::renderPieces(mat4 vp)
{
    // Bind the program and VAO
    glUseProgram(piece_program_);
    glBindVertexArray(piece_vao_);

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
                PieceType pt = board_->getPiece(mailbox(i,j,k)).type();
                float hue = (int) pt * (360.0f / 16);
                vec3 color = glm::rgbColor(vec3(hue, 1, 1));

                // Set uniform variables
                GLuint vp_loc = glGetUniformLocation(piece_program_, "VP");
                GLuint m_loc = glGetUniformLocation(piece_program_, "M");
                GLuint color_loc = glGetUniformLocation(piece_program_, "color");
                glUniformMatrix4fv(vp_loc, 1, false, glm::value_ptr(vp));
                glUniformMatrix4fv(m_loc, 1, false, glm::value_ptr(model));
                glUniform3f(color_loc, color.r, color.g, color.b);

                // Find the piece model in the VBO and draw!
                int offset = PIECE_MODEL_OFFSET[pt];
                int length = PIECE_MODEL_LENGTH[pt];
                glDrawArrays(GL_TRIANGLES, offset, length);
            }
        }
    }

    // Unbind everything
    glBindVertexArray(0);
    glUseProgram(0);
}
