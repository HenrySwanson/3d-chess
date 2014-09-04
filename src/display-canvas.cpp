#include "display-canvas.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

DisplayCanvas::DisplayCanvas(wxWindow *parent) : wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"), wxNullPalette)
{
    context_ = new wxGLContext(this); // TODO possibly destruct?

    Connect(GetId(), wxEVT_PAINT, wxPaintEventHandler(DisplayCanvas::render));
}

void DisplayCanvas::initializeOpenGL()
{
    GLenum err = glewInit();
    if(err != GLEW_OK)
        std::cout << "GLEW failed to initialize" << std::endl;

    program_ = makeProgram();
    grid_vao_ = makeGridVao(program_);
}

void DisplayCanvas::render(wxPaintEvent& evt)
{
    SetCurrent(*context_);
    wxPaintDC(this);

    static bool initialized = false;
    if(!initialized)
    {
        initializeOpenGL();
        initialized = true;
    }

    wxSize size = GetClientSize();
    glViewport(0, 0, size.x, size.y);

    camera_ = glm::lookAt(glm::vec3(3,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
    projection_ = glm::perspective<float>(50.0, (float)size.x/size.y, 0.1, 10.0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program_);

    GLuint camera_loc = glGetUniformLocation(program_, "camera");
    GLuint projection_loc = glGetUniformLocation(program_, "projection");

    glUniformMatrix4fv(camera_loc, 1, false, glm::value_ptr(camera_));
    glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection_));

    glBindVertexArray(grid_vao_);
    glDrawArrays(GL_LINES, 0, 2 * 81 * 3);
    glBindVertexArray(0);
    glUseProgram(0);

    SwapBuffers();
}
