#include <iostream>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "camera.h"

int screen_width, screen_height;

Camera camera;

GLfloat rotation_x_matrix[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };
GLfloat rotation_y_matrix[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };

/* Callback functions */
void
window_resize_callback (GLFWwindow *window,
                               int width,
                               int height)
{
        screen_width  = width;
        screen_height = height;

        glViewport(0, 0, screen_width, screen_height);
}

void
cursor_position_callback (GLFWwindow *window,
                              double xpos,
                              double ypos)
{
        static GLfloat lastX = 400, lastY = 300;  // Типа центр экрана

        static bool firstMouse = true;        
        if (firstMouse) // эта переменная была проинициализирована значением true
        {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
        }
        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos; // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
        lastX = xpos;
        lastY = ypos;

        camera.process_mouse_movement(xoffset, yoffset);
}

GLfloat deltaTime = 0.0f;    // Время, прошедшее между последним и текущим кадром
bool keys[1024];

void
do_movement()
{
  // Camera controls
  if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(MOV_FORWARD, deltaTime);
  if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(MOV_BACKWARD, deltaTime);
  if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(MOV_LEFT, deltaTime);
  if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(MOV_RIGHT, deltaTime);
}

void
key_callback(GLFWwindow *window,
                    int key, 
                    int scancode, 
                    int action,
                    int mode)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if(action == GLFW_PRESS)
                keys[key] = true;
        else if(action == GLFW_RELEASE)
                keys[key] = false;
}

GLuint
get_vertex_array_object (void) {
    GLfloat vertices[] = {
        // Позиции
         1.0f,  1.0f, 0.0f,   // Верхний правый угол
         1.0f, -1.0f, 0.0f,   // Нижний  правый угол
        -1.0f, -1.0f, 0.0f,   // Нижний  левый  угол
        -1.0f,  1.0f, 0.0f,   // Верхний левый  угол
        };
        
    GLuint indices[] = {  // Помните, что мы начинаем с 0!
        0, 1, 3,   // Первый треугольник
        1, 2, 3    // Второй треугольник
    };

    GLuint VBO, EBO, VAO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
        
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        /* Настройка параметров вершинного шейдера */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
       
    /* Отвязываем VAO*/
    glBindVertexArray(0);

    return VAO;
}

/* Main function*/
int
main(int argc, char** argv)
{
        //Инициализация GLFW
        glfwInit();
        //Настройка GLFW
        //Задается минимальная требуемая версия OpenGL. 
        //Мажорная 
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        //Минорная
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //Установка профайла для которого создается контекст
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //Включение возможности изменения размера окна
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


        GLFWwindow *window = glfwCreateWindow(800, 600, "Mashgraph 1", nullptr, nullptr);
        if (window == nullptr) {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return -1;
        }
        glfwMakeContextCurrent(window);


        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
                std::cout << "Failed to initialize GLEW" << std::endl;
                return -1;
        }


        
        glfwGetFramebufferSize(window, &screen_width, &screen_height);
        glViewport(0, 0, screen_width, screen_height);

        /* Путь к файлам */
        std::string base_path(argv[0]);
        base_path = base_path.substr(0, base_path.find_last_of("/") + 1);
        std::string vertex_path(base_path + "shaders/shader.vs");
        std::string fragment_path(base_path + "shaders/shader.fs");

        Shader myShader(vertex_path.c_str(), fragment_path.c_str());
        GLuint VAO = get_vertex_array_object();

        /* Обработчики событий */
        glfwSetKeyCallback        (window, key_callback);
        glfwSetWindowSizeCallback (window, window_resize_callback);
        glfwSetCursorPosCallback  (window, cursor_position_callback);
        
        /* Игровой цикл */
        GLfloat lastFrame = 0.0f;
        while(!glfwWindowShouldClose(window)) {
                glfwPollEvents();
                do_movement();

                /* Передача параметров */
                myShader.set_uniform("iResolution", screen_width, screen_height);
                myShader.set_uniform("iTime", glfwGetTime());
                myShader.set_uniform_vector("camera_position", glm::value_ptr(camera.get_position()), 3);
                myShader.set_uniform_matrix("transform_matrix", glm::value_ptr(camera.get_transform()), 4);

                myShader.use();

                glBindVertexArray(VAO);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glBindVertexArray(0);

                glfwSwapBuffers(window);

                /* Меняем время */
                GLfloat currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;
        }


        glfwTerminate();
        return 0;
}
