#ifndef LASER_H_
#define LASER_H_

#include <GL/glew.h>
#include "../glm/glm.hpp"

#include "object.h"
#include "../shader.h"
#include "../vao.h"
#include "../texture2d.h"

class Laser : public Object
{
public:
        GLfloat radius;
        Laser (const glm::vec3 &point, const glm::vec3 &direction, const glm::vec3 &color = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat speed = 10.0);
        virtual void draw (const glm::mat4 &view, const glm::mat4 &projection);
protected:
        static const GLchar *v_path;
        static const GLchar *f_path;
        static const GLchar *t_path;
        static GLfloat vertices[];

        static Shader    *shader;
        static Vao       *vao;
        static Texture2D *texture;


        glm::vec3 color;

        virtual glm::mat4 get_model_matrix (void);
};

#endif