#include "shader.h"

Shader :: Shader (const GLchar *vertexPath, const GLchar *fragmentPath)
{
    /* Получаем код в си-строку */
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();

    /* Компилируем эти строки */
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    /* Вершинный шейдер */
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Если есть ошибки - вывести их
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    /* Фрагментный шейдер */
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Если есть ошибки - вывести их
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    /* Шейдерная программа */
    this->program = glCreateProgram();
    glAttachShader(this->program, vertex);
    glAttachShader(this->program, fragment);
    glLinkProgram(this->program);
    //Если есть ошибки - вывести их
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Удаляем шейдеры, поскольку они уже в программу и нам больше не нужны.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void
Shader :: use ()
{
    glUseProgram(this->program);
}

GLint
Shader :: get_uniform_location (const GLchar *variable)
{
    return glGetUniformLocation(this->program, variable);
}

void
Shader :: set_uniform (const GLchar *variable, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
    GLint location = get_uniform_location(variable);
    glUniform4f(location, f1, f2, f3, f4);
}

void
Shader :: set_uniform (const GLchar *variable, GLfloat f1, GLfloat f2)
{
    GLint location = get_uniform_location(variable);
    glUniform2f(location, f1, f2);
}

void
Shader :: set_uniform (const GLchar *variable, GLfloat f)
{
    GLint location = get_uniform_location(variable);
    glUniform1f(location, f);
}