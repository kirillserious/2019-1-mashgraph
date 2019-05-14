#include "texture2d.h"

#include "stb/stb_image.h"

#include <stdexcept>

Texture2D :: Texture2D (const GLchar *image_path)
{
        set_up_texture(image_path);
}

Texture2D :: Texture2D (const std::string &image_path)
{
        set_up_texture(image_path.c_str());
}

void
Texture2D :: set_up_texture (const GLchar *image_path)
{
        /* Генерация текстуры */
        glGenTextures(1, &texture);
        
        /* Привязка текстуры к рабочей области */
        glBindTexture(GL_TEXTURE_2D, texture);
        /* Устанавливаем настройки фильтрации и преобразований (на текущей текстуре)*/
        // ...
        
        /* Загружаем и генерируем текстуру */
        int width, height, channels_number;


        unsigned char *data = stbi_load(
                        image_path,
                        &width,
                        &height,
                        &channels_number,
                        0);
        if (!data) {
                throw std::runtime_error("Cubemap texture failed to load in skybox");
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);

        glGenerateMipmap(GL_TEXTURE_2D);

        /* Отвязываем текстуру от рабочей области */
        glBindTexture(GL_TEXTURE_2D, 0);
}