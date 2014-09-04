#ifndef CHESS_OPENGLHELPER_H
#define CHESS_OPENGLHELPER_H

#include <GL/glew.h>

GLuint makeProgram();

GLuint makeVao(GLuint program, GLuint vbo);

GLuint makeVbo();

GLuint makeBoth(GLuint program);

GLuint makeGridVao(GLuint program);

#endif
