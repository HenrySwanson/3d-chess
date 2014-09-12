#ifndef CHESS_OPENGLHELPER_H
#define CHESS_OPENGLHELPER_H

#include <GL/glew.h>

#include <vector>

/**
 * Loads vertex and fragment shader code from files, compiles them, then links
 * them into a shader program. Returns the ID of this program.
 */
GLuint makeProgram(const char* vert_filename, const char* frag_filename);

/**
 * Given a string ("foo"), finds the file resources/foo.obj, loads it, then
 * returns a vector with the faces.
 */
std::vector<float> loadObjFile(const char* obj_filename);

#endif
