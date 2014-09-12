#include "opengl-helper.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

GLuint loadShaderFromFile(const char* filePath, GLenum type)
{
    std::ifstream in_file;
    in_file.open(filePath, std::ios::in);

    if(!in_file.is_open())
        std::cout << "Failed to open file: " << filePath << std::endl;

    // Read the file into the stringstream
    std::stringstream ss;
    ss << in_file.rdbuf();

    // And pop it out as a C-string
    std::string str = ss.str();
    const char* c_str = str.c_str();

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

GLuint makeProgram(const char* vert_filename, const char* frag_filename)
{
    // Load both (compiled) shaders
    GLuint vert_shader = loadShaderFromFile(vert_filename, GL_VERTEX_SHADER);
    GLuint frag_shader = loadShaderFromFile(frag_filename, GL_FRAGMENT_SHADER);

    // Generate program, attach shaders and link together
    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    // Detach and delete shaders
    glDetachShader(program, vert_shader);
    glDetachShader(program, frag_shader);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

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

std::vector<float> loadObjFile(const char* obj_filename)
{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> faces;

    std::stringstream ss;
    ss << "resources/" << obj_filename << ".obj";
    std::string path = ss.str();

    FILE * file = fopen(path.c_str(), "r");

    if( file == NULL )
    {
        std::cout << "Failed to open file: " << path << std::endl;
        return std::vector<float>(0);
    }

    while(true)
    {
        char line_header [128];
        int res = fscanf(file, "%s", line_header);
        if(res == EOF)
            break;

        if(strcmp(line_header, "v") == 0)
        {
            float x, y, z;
            fscanf(file, "%f %f %f\n", &x, &y, &z);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        else if(strcmp(line_header, "vn") == 0)
        {
            float x, y, z;
            fscanf(file, "%f %f %f\n", &x, &y, &z);
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
        else if(strcmp(line_header, "f") == 0)
        {
            unsigned int v_indices [3];
            unsigned int n_indices [3];
            int matches = fscanf(file, "%d//%d %d//%d %d//%d\n",
                    &v_indices[0], &n_indices[0],
                    &v_indices[1], &n_indices[1],
                    &v_indices[2], &n_indices[2]);

            if(matches != 6)
            {
                std::cout << "Could not parse file with this dumb parser: " <<
                        path << std::endl;
                return std::vector<float>(0);
            }

            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                    faces.push_back(vertices[3 * (v_indices[i] - 1) + j]);
                //for(int j = 0; j < 3; j++)
                    //faces.push_back(normals[3 * (n_indices[i] - 1) + j]);
            }
        }
    }

    return faces;
}
