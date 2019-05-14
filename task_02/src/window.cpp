#include "window.h"
#include <stdexcept>

bool         Window::is_initialized = false;
GLFWwindow  *Window::window         = nullptr;
GLint        Window :: width        = 0;
GLint        Window :: height       = 0;

void 
Window::create_window (const GLchar *title, GLint width, GLint height, GLboolean is_resizable)
{
        /* Проверка не инициалировали ли окно раньше */
        if (is_initialized) {
                throw std::runtime_error("Trying to create second window.");
        }         
        is_initialized = true;
        
        /* Инициализация GLFW */
        glfwInit();
        
        /* Настройка GLFW */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_RESIZABLE, is_resizable);

        /* Cоздание окна */
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (window == nullptr) {
                glfwTerminate();
                throw std::runtime_error("Failed to create GLFW window");
        }

        /* Установка контекста для инициализации GLEW */
        glfwMakeContextCurrent(window);

        /* Инициализация GLEW */
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
                throw std::runtime_error("Failed to initialize GLEW");
        }

        /* Отрисовка окна */
        glfwGetFramebufferSize(window, & Window::width, & Window::height);
        glViewport(0, 0, Window::width, Window::height);

        /* Установка стандартных коллбэков */
        glfwSetWindowSizeCallback(window, window_resize_callback);
}

void
Window :: delete_window (void)
{
        glfwTerminate();
}

void
Window :: window_resize_callback (GLFWwindow *window, GLint width, GLint height)
{
        Window::width  = width;
        Window::height = height;
        glViewport(0, 0, width, height);
}

GLint
Window :: get_width  (void)
{
        return width;
}

GLint
Window :: get_height (void)
{
        return height;
}
