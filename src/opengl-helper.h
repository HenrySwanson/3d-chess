#ifndef CHESS_OPENGLHELPER_H
#define CHESS_OPENGLHELPER_H

#include <GL/glew.h>

#include <vector>

/**
 * Loads vertex and fragment shader code from files, compiles them, then links
 * them into a shader program. Returns the ID of this program.
 */
GLuint makeProgram(const char* vert_shader_src, const char* frag_shader_src);

std::vector<float> loadObjFile(const char* obj_model_src);

#endif
