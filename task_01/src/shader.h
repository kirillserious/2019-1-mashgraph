#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
        GLuint program;

        Shader (const GLchar *vertexPath, const GLchar *fragmentPath);

        void use();


        GLint get_uniform_location (const GLchar *variable);
        void  set_uniform          (const GLchar *variable, GLfloat f);
       	void  set_uniform          (const GLchar *variable, GLfloat f1, GLfloat f2);
       	void  set_uniform          (const GLchar *variable, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);

        void  set_uniform_vector   (const GLchar *variable, const GLfloat *vector, int shape);
        void  set_uniform_matrix   (const GLchar *variable, const GLfloat *matrix, int shape);

};

#endif
