#ifndef WINDOW_H_
#define WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
        static bool  is_initialized;
        static GLint width;
        static GLint height;

        static void window_resize_callback (GLFWwindow *window, GLint width, GLint height);

public:

        static GLFWwindow *window; // private

        static void  create_window (const GLchar *title, GLint width, GLint height, GLboolean is_resizable = GL_TRUE);
        static void  delete_window (void);

        static GLint get_width  (void);
        static GLint get_height (void);
};

#endif