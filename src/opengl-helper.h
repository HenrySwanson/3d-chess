#ifndef CHESS_OPENGLHELPER_H
#define CHESS_OPENGLHELPER_H

#include <GL/glew.h>

#include <vector>

/**
 * Given two strings ("foo", "bar"), loads vertex and fragment shader code from
 * the files resources/foo.vertexshader and resources/bar.fragmentshader,
 * compiles them, then links them into a shader program. Returns the ID of
 * this program.
 */
GLuint makeProgram(const char* vert_filename, const char* frag_filename);

/**
 * Given a string ("foo"), finds the file resources/foo.obj, loads it, then
 * returns a vector with the faces.
 */
std::vector<float> loadObjFile(const char* obj_filename);

#endif
