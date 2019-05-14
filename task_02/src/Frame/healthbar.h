#ifndef HEALTH_BAR_H_
#define HEALTH_BAR_H_

#include <GL/glew.h>
#include "../shader.h"
#include "../vao.h"


class HealthBar
{
        inline static const std::string vshader_path = "../shaders/frame/healthbar.vs";
        inline static const std::string fshader_path = "../shaders/frame/healthbar.fs";

        static GLfloat vertices[], frame_vertices[];
        static Shader *shader;
        static Vao *vao, *frame_vao;
        static int count;

public:
        GLfloat health;

        HealthBar(GLfloat health=1.);
        ~HealthBar(void);
        void draw();
};

#endif