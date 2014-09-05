#include "opengl-helper.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

GLuint loadShaderFromFile(const char* filePath, GLenum type)
{
    std::ifstream in_file;
    in_file.open(filePath, std::ios::in | std::ios::binary);

    if(!in_file.is_open())
        std::cout << "Failed to open file: " << filePath << std::endl;

    // Read the file into the stringstream
    std::stringstream ss;
    ss << in_file.rdbuf();

    // And pop it out as a C-string
    const char* c_str = ss.str().c_str();

    // Generate the shader, load the source code, and compile
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);

    // Make sure compilation succeeded
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

GLuint makeProgram(const char* vert_shader_src, const char* frag_shader_src)
{
    // Load both (compiled) shaders
    GLuint vert_shader = loadShaderFromFile(vert_shader_src, GL_VERTEX_SHADER);
    GLuint frag_shader = loadShaderFromFile(frag_shader_src, GL_FRAGMENT_SHADER);

    // Generate program, attach shaders and link together
    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    // Make sure linking succeeded
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        std::cout << "Program did not link!" << std::endl;
        return 0;
    }

    return program;   
}
