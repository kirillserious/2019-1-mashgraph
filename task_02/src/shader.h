#ifndef SHADER_H_
#define SHADER_H_

#include <string>

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "texture2d.h"

class Shader
{
        void set_up_shader        (const GLchar *v_path, const GLchar *f_path, const GLchar *g_path = nullptr);
        void check_compile_errors (GLuint shader, const std::string &type);

        GLint get_uniform_location (const GLchar *variable);

public:
        GLuint program; // public for Mesh

        Shader (const GLchar *v_path, const GLchar *f_path, const GLchar *g_path = nullptr);
        Shader (const std::string &v_path, const std::string &f_path);
        Shader (const std::string &v_path, const std::string &f_path, const std::string &g_path);

        void use();
        
        void  set_uniform  (const GLchar *variable, GLfloat f);
       	void  set_uniform  (const GLchar *variable, GLfloat f1, GLfloat f2);
        void  set_uniform  (const GLchar *variable, GLfloat f1, GLfloat f2, GLfloat f3);
       	void  set_uniform  (const GLchar *variable, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);
       	void  set_uniform  (const GLchar *variable, const glm::vec2 &vector);
        void  set_uniform  (const GLchar *variable, const glm::vec3 &vector);
        void  set_uniform  (const GLchar *variable, const glm::vec4 &vector);
        void  set_uniform  (const GLchar *variable, const glm::mat2 &matrix);
       	void  set_uniform  (const GLchar *variable, const glm::mat3 &matrix);
       	void  set_uniform  (const GLchar *variable, const glm::mat4 &matrix);

       	void  set_uniform_texture  (const GLchar *variable, Texture2D &texture, int place);

};

#endif
