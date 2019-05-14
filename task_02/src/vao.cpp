#include "vao.h"

Vao :: Vao (GLfloat *vertices, GLint vertices_number, GLint vertex_size)
: vertices_number(vertices_number), vertex_size(vertex_size)
{
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
    
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_number*vertex_size*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
}

void
Vao :: set_location (GLint location, GLint size, GLint offset)
{
        glBindVertexArray(vao);
        glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, vertex_size*sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
        glEnableVertexAttribArray(location);         
}

void
Vao :: draw (void)
{
        draw_as_triangles();
}

void
Vao :: draw_as_triangles (void)
{
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices_number);
}

void
Vao :: draw_as_lines (GLuint thickness)
{
        glBindVertexArray(vao);
        glLineWidth(thickness);
        glDrawArrays(GL_LINES, 0, vertices_number);
        glLineWidth(0);
}