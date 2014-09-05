#ifndef CHESS_OPENGLHELPER_H
#define CHESS_OPENGLHELPER_H

#include <GL/glew.h>

/**
 * Loads vertex and fragment shader code from files, compiles them, then links
 * them into a shader program. Returns the ID of this program.
 */
GLuint makeProgram(const char* vert_shader_src, const char* frag_shader_src);

#endif
