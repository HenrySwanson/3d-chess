#include "display-canvas.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

// Pi
static const float PI = 3.1415926535f;

// Distance of the camera from the board center
static const float EYE_RAD = 15;

// Attributes to set up the OpenGL context
static int OPEN_GL_ATTRIBS [] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

DisplayCanvas::DisplayCanvas(wxWindow *parent) : wxGLCanvas(parent, wxID_ANY, OPEN_GL_ATTRIBS, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"), wxNullPalette)
{
    context_ = new wxGLContext(this); // TODO possibly destruct?

    theta_ = 0;
    phi_ = PI/2;

    // Connect events to their handlers
    Connect(GetId(), wxEVT_LEFT_DOWN, wxMouseEventHandler(DisplayCanvas::handleMouseDown));
    Connect(GetId(), wxEVT_MOTION, wxMouseEventHandler(DisplayCanvas::handleMouseDrag));
    Connect(GetId(), wxEVT_PAINT, wxPaintEventHandler(DisplayCanvas::render));
}

void DisplayCanvas::handleMouseDown(wxMouseEvent& evt)
{
    old_mouse_pos_ = evt.GetPosition();
}

void DisplayCanvas::handleMouseDrag(wxMouseEvent& evt)
{
    if(!evt.LeftIsDown())
        return;

    wxPoint delta = evt.GetPosition() - old_mouse_pos_;
    old_mouse_pos_ += delta;

    theta_ -= (delta.x * PI / 200);
    phi_ -= (delta.y * PI / 200);

    theta_ = glm::mod(theta_, 2 * PI);
    phi_ = glm::clamp(phi_, 0.001f, PI - 0.001f);

    Refresh();
}

void DisplayCanvas::initializeOpenGL()
{
    GLenum err = glewInit();
    if(err != GLEW_OK)
        std::cout << "GLEW failed to initialize!" << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    glEnable(GL_CULL_FACE);

    grid_program_ = makeProgram("shaders/grid.vertexshader", "shaders/grid.fragmentshader");
    piece_program_ = makeProgram("shaders/piece.vertexshader", "shaders/piece.fragmentshader");

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
    
    // Configure the "vert" variable of the vertex shader
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
    GLfloat vertexData [3 * 3 * 4] = {
        0.8, 0.2, 0.0,    0.2, 0.2, 0.0,    0.5, 0.8, 0.0,
        0.8, 0.2, 0.0,    0.5, 0.8, 0.0,    0.5, 0.5, 0.5,
        0.5, 0.8, 0.0,    0.2, 0.2, 0.0,    0.5, 0.5, 0.5,
        0.2, 0.2, 0.0,    0.8, 0.2, 0.0,    0.5, 0.5, 0.5
    };

    // Load that array into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Configure the "vert" variable of the vertex shader
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
    glm::vec3 eye = glm::vec3(EYE_RAD * sin(phi_) * cos(theta_),
                              EYE_RAD * sin(phi_) * sin(theta_),
                              EYE_RAD * cos(phi_));

    // and sets up the appropriate matrices.
    glm::mat4 view = glm::lookAt(eye, glm::vec3(0,0,0), glm::vec3(0,0,1));
    glm::mat4 proj = glm::perspective<float>(50.0, screen_x / screen_y, 8.0, 20.0);

    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 vp = proj * view;

    renderGrid(vp);
    renderPieces(vp);

    // Display our brand-new picture!
    SwapBuffers();
}

void DisplayCanvas::renderGrid(glm::mat4 vp)
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

void DisplayCanvas::renderPieces(glm::mat4 vp)
{
    glUseProgram(piece_program_);

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                glm::vec3 corner = glm::vec3(i - 4, j - 4, k - 4);
                glm::mat4 model = glm::translate(glm::mat4(), corner);

                // Bind uniform variables
                GLuint vp_loc = glGetUniformLocation(piece_program_, "VP");
                GLuint m_loc = glGetUniformLocation(piece_program_, "M");
                GLuint color_loc = glGetUniformLocation(piece_program_, "color");
                glUniformMatrix4fv(vp_loc, 1, false, glm::value_ptr(vp));
                glUniformMatrix4fv(m_loc, 1, false, glm::value_ptr(model));
                glUniform3f(color_loc, 1.0f, 0.0f, 0.0f);

                // Bind the VAO and draw
                glBindVertexArray(piece_vao_);
                glDrawArrays(GL_TRIANGLES, 0, 12);
            }
        }
    }

    // Unbind everything
    glBindVertexArray(0);
    glUseProgram(0);
}
