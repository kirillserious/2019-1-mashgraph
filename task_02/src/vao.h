#ifndef VAO_H_
#define VAO_H_

#include <GL/glew.h>

class Vao
{
        GLint    vertices_number;
        GLint    vertex_size;

        GLuint vao; 
        GLuint vbo; 
public:


        Vao (GLfloat *vertices, GLint vertices_number, GLint vertex_size);

        void  set_location (GLint location, GLint size, GLint offset);
        
        void  draw              (void);
        void  draw_as_triangles (void);
        void  draw_as_lines     (GLuint thickness = 1.0f);
};

#endif