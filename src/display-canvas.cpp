#include "display-canvas.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Distance of the camera from the board center
static const float EYE_RAD = 3;

DisplayCanvas::DisplayCanvas(wxWindow *parent) : wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"), wxNullPalette)
{
    context_ = new wxGLContext(this); // TODO possibly destruct?

    theta_ = 0;
    phi_ = 1.57079632679; // pi/2

    // Connect events to their handlers
    Connect(GetId(), wxEVT_PAINT, wxPaintEventHandler(DisplayCanvas::render));
}

void DisplayCanvas::initializeOpenGL()
{
    GLenum err = glewInit();
    if(err != GLEW_OK)
        std::cout << "GLEW failed to initialize!" << std::endl;

    grid_program_ = makeProgram("shaders/vertex-shader.txt", "shaders/fragment-shader.txt");

    initializeGrid();
}

void DisplayCanvas::initializeGrid()
{
    // Make and bind the VAO and VBO
    glGenVertexArrays(1, &grid_vao_);
    glBindVertexArray(grid_vao_);
    glGenBuffers(1, &grid_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, grid_vbo_);

    // Write vertices for the lines of the grid
    GLfloat vertexData [3 * 2 * 81 * 3];
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            int index = (i * 9 + j) * 3 * 2 * 3;
            GLfloat a = i * 0.25f - 1.0f;
            GLfloat b = j * 0.25f - 1.0f;
            GLfloat lines [18] = {
                a, b,-1,  a, b, 1,
                a,-1, b,  a, 1, b,
               -1, a, b,  1, a, b
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
    glm::vec3 eye = glm::vec3(EYE_RAD * cos(phi_) * cos(theta_),
                              EYE_RAD * cos(phi_) * sin(theta_),
                              EYE_RAD * sin(phi_));

    // and sets up the appropriate matrices.
    camera_ = glm::lookAt(eye, glm::vec3(0,0,0), glm::vec3(0,0,1));
    projection_ = glm::perspective<float>(50.0, screen_x / screen_y, 0.1, 10.0);

    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    renderGrid();

    // Display our brand-new picture!
    SwapBuffers();
}

void DisplayCanvas::renderGrid()
{
    glUseProgram(grid_program_);

    // Bind uniform variables
    GLuint camera_loc = glGetUniformLocation(grid_program_, "camera");
    GLuint projection_loc = glGetUniformLocation(grid_program_, "projection");
    glUniformMatrix4fv(camera_loc, 1, false, glm::value_ptr(camera_));
    glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection_));

    // Bind the VAO and draw
    glBindVertexArray(grid_vao_);
    glDrawArrays(GL_LINES, 0, 2 * 81 * 3);

    // Unbind everything
    glBindVertexArray(0);
    glUseProgram(0);
}
