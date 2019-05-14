#ifndef TEXTURE2D_H_
#define TEXTURE2D_H_

#include <GL/glew.h>

#include <string>

class Texture2D
{
        friend class Shader;
        
private:
        GLuint texture;

        void set_up_texture (const GLchar *image_path);
public:
        Texture2D (const GLchar *image_path);
        Texture2D (const std::string &image_path);

};

#endif