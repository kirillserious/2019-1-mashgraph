#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <vector>
#include <string>
#include <stdexcept>

#include <GL/glew.h>

#include "glm/glm.hpp"

#include "stb/stb_image.h"

#include "shader.h"
#include "vao.h"

class Skybox
{
        inline static const std::string vshader_path = "../shaders/skybox.vs";
        inline static const std::string fshader_path = "../shaders/skybox.fs";

        static GLuint texture_id;
        static GLfloat vertices[];
        
        static Shader *shader;
        static Vao    *vao;
public:
        static  void  load  (const std::vector<std::string> &faces);
        static  void  draw  (const glm::mat4 &view, const glm::mat4 &projection);
};

#endif