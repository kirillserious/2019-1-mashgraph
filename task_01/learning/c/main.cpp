#include <iostream>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
}

GLuint
get_vertex_array_object (void) {
    GLfloat vertices[] = {
        // Позиции          // Цвета
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Верхний правый угол
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Нижний правый угол
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // Нижний левый угол
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f   // Верхний левый угол
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);        
    /* Отвязываем VAO*/
    glBindVertexArray(0);

    return VAO;
}

GLuint
get_shader_program (void) {
    /* Компиляция вершинного шейдера */
        GLchar const* vertexShaderSource = {
                "#version 330 core \n"

                "layout (location = 0) in vec3 position;"
                "layout (location = 1) in vec3 color;"
                
                "out vec4 vertexColor;"

                "void main()"
                "{"
                "    gl_Position = vec4(position, 1.0);"
                "    vertexColor = vec4(color, 1.0);" 
                "}"
        };

        GLuint vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        /* Компиляция фрагментного шейдера */
        GLchar const* fragmentShaderSource = {
                "#version 330 core \n"

                "in  vec4 vertexColor;"
                "out vec4 color;"
                "uniform vec4 ourColor;"

                "void main()"
                "{"
                "        color = ourColor + vertexColor;" 
                "}"
        };

        GLuint fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        /* Линковка шейдеров с шейдерной программой */
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        /* Запуск программы, удаление шейдеров */
        //glUseProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
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
        //Выключение возможности изменения размера окна
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


        GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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


        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        GLuint shaderProgram = get_shader_program();
        GLuint VAO = get_vertex_array_object();

        glfwSetKeyCallback(window, key_callback);
        while(!glfwWindowShouldClose(window)) {
                glfwPollEvents();

                GLfloat timeValue  = glfwGetTime();
                GLfloat greenValue = (sin(timeValue)/2) + 0.5;
                GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
                glUseProgram(shaderProgram);
                glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

                glBindVertexArray(VAO);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glBindVertexArray(0);

                glfwSwapBuffers(window);
        }


        glfwTerminate();
        return 0;
}