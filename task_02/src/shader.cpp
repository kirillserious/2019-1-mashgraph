#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

Shader :: Shader (const GLchar *v_path, const GLchar *f_path, const GLchar *g_path)
{
        set_up_shader(v_path, f_path, g_path);
}

Shader :: Shader (const std::string &v_path, const std::string &f_path)
{
        set_up_shader(v_path.c_str(), f_path.c_str(), nullptr);
}
Shader :: Shader (const std::string &v_path, const std::string &f_path, const std::string &g_path)
{
        set_up_shader(v_path.c_str(), f_path.c_str(), g_path.c_str());
}

void
Shader :: set_up_shader (const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath)
{
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
                // open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                // read file's buffer contents into streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();       
                // close file handlers
                vShaderFile.close();
                fShaderFile.close();
                // convert stream into string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();         
                // if geometry shader path is present, also load a geometry shader
                if (geometryPath != nullptr) {
                        gShaderFile.open(geometryPath);
                        std::stringstream gShaderStream;
                        gShaderStream << gShaderFile.rdbuf();
                        gShaderFile.close();
                        geometryCode = gShaderStream.str();
                }
        }
        catch (std::ifstream::failure e) {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        check_compile_errors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        check_compile_errors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if (geometryPath != nullptr) {
                const char * gShaderCode = geometryCode.c_str();
                geometry = glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(geometry, 1, &gShaderCode, NULL);
                glCompileShader(geometry);
                check_compile_errors(geometry, "GEOMETRY");
        }
        // shader Program
        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        if (geometryPath != nullptr) {
                glAttachShader(program, geometry);
        }
        glLinkProgram(program);
        check_compile_errors(program, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr) {
                glDeleteShader(geometry);
        }
}

void
Shader :: check_compile_errors(GLuint shader, const std::string &type)
{
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
        } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
        }
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
Shader :: set_uniform (const GLchar *variable, GLfloat f1, GLfloat f2, GLfloat f3)
{
        GLint location = get_uniform_location(variable);
        glUniform3f(location, f1, f2, f3);
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

void
Shader :: set_uniform (const GLchar *variable, const glm::mat2 &matrix)
{
        GLint location = get_uniform_location(variable);
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void
Shader :: set_uniform (const GLchar *variable, const glm::mat3 &matrix)
{
        GLint location = get_uniform_location(variable);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void
Shader :: set_uniform (const GLchar *variable, const glm::mat4 &matrix)
{
        GLint location = get_uniform_location(variable);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void
Shader :: set_uniform (const GLchar *variable, const glm::vec3 &vector)
{
        GLint location = get_uniform_location(variable);
        glUniform3fv(location, 1, glm::value_ptr(vector));
}

void
Shader :: set_uniform (const GLchar *variable, const glm::vec2 &vector)
{
        GLint location = get_uniform_location(variable);
        glUniform2fv(location, 1, glm::value_ptr(vector));
}

void
Shader :: set_uniform (const GLchar *variable, const glm::vec4 &vector)
{
        GLint location = get_uniform_location(variable);
        glUniform4fv(location, 1, glm::value_ptr(vector));
}

void
Shader :: set_uniform_texture (const GLchar *variable, Texture2D &texture, GLint place)
{
        if (place > GL_MAX_TEXTURE_UNITS) {
                throw std::runtime_error("SHADER::SET_UNIFORM::TEXTURE::  Too big number of textures");
        }

        glActiveTexture (GL_TEXTURE0 + place);
        glBindTexture   (GL_TEXTURE_2D, texture.texture);
        GLuint location = get_uniform_location(variable);
        glUniform1i(location, place);
}
