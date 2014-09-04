#include "opengl-helper.h"

#include <iostream>
#include <cstring>

static const GLchar * vert_shader_src = "#version 150 \n uniform mat4 projection; uniform mat4 camera; in vec3 vert; void main() { gl_Position = projection * camera * vec4(vert, 1); }";
static const GLchar * frag_shader_src = "#version 150 \n out vec4 finalColor; void main() { finalColor = vec4(1.0, 1.0, 1.0, 1.0); }";

GLuint makeShader(GLenum type, const GLchar * src_code)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &src_code, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        std::cout << "Shader did not compile!" << std::endl;

        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        std::cout << strInfoLog << std::endl;

        return 0;
    }

    return shader;
}

// TODO add error checking
GLuint makeProgram()
{
    GLuint vert_shader = makeShader(GL_VERTEX_SHADER, vert_shader_src);
    GLuint frag_shader = makeShader(GL_FRAGMENT_SHADER, frag_shader_src);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        std::cout << "Program did not link!" << std::endl;
        return 0;
    }

    return program;   
}

GLuint makeGridVao(GLuint program)
{
    GLuint vao, vbo;

    // Make and bind the VAO and VBO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Write vertices for the lines of the grid
    GLfloat vertexData [3 * 2 * 81 * 3];
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            int index = (i * 9 + j) * 3 * 2 * 3;
            GLfloat a = i * 0.25f - 1.0f;
            GLfloat b = j * 0.25f - 1.0f;
            GLfloat lines [] = {
                a, b,-1,  a, b, 1,
                a,-1, b,  a, 1, b,
               -1, a, b,  1, a, b
            };
            memcpy(vertexData + index, lines, 18 * sizeof(float));
        }
    }

    // Load that array into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    
    // connect the xyz to the "vert" attribute of the vertex shader
    GLuint in_vert = glGetAttribLocation(program, "vert");
    glEnableVertexAttribArray(in_vert);
    glVertexAttribPointer(in_vert, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vao;
}
