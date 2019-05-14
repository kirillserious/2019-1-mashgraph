#include "skybox.h"
#include "fs.h"

GLuint Skybox :: texture_id = 0;

Shader *Skybox:: shader = nullptr;
Vao    *Skybox:: vao    = nullptr; 

GLfloat Skybox ::vertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


void
Skybox :: load (const std::vector<std::string> &faces)
{
        /* Генерация кубической карты */
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

        /* Загрузка изображения*/
        int width, height, channels_number;

        for (unsigned int i = 0; i < faces.size(); ++i) {
                unsigned char *data = stbi_load(
                                faces[i].c_str(),
                                &width,
                                &height,
                                &channels_number,
                                0);
                if (!data) {
                        throw std::runtime_error("Cubemap texture failed to load in skybox");
                }
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
        }
        /* Установка параметров текстуры */
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        /* Загрузка шейдеров */
        if (!shader) {
                shader = new Shader(Fs::make(vshader_path), Fs::make(fshader_path));
        }
        /* Создание Vao */
        if (!vao) {
                vao = new Vao(vertices, 36, 3);
                vao->set_location(0, 3, 0);
        }
}

void
Skybox :: draw (const glm::mat4 &view, const glm::mat4 &projection)
{
        glDepthFunc(GL_LEQUAL);
        shader->use();
        shader->set_uniform("view", glm::mat4(glm::mat3(view)));  // Из матрицы view убрана информация о движении
        shader->set_uniform("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
        
        vao->draw();
        glDepthFunc(GL_LESS);
}