#ifndef OBJECT_H_
#define OBJECT_H_

#include <GL/glew.h>
#include "../glm/glm.hpp"
#include "../shader.h"

class Object
{
protected:
        /* Характеристики шейдера */
        Shader *shader;

        /* Характеристики объекта */
        glm::vec3 point;
        glm::vec3 direction;
        GLfloat   speed;

        Object
        (
                const glm::vec3 &point,
                const glm::vec3 &direction,
                GLfloat speed,
                Shader *shader,
                const GLchar *v_path,
                const GLchar *f_path,
                const GLchar *g_path = nullptr
        ) : shader(shader), point(point), direction(direction), speed(speed)
        {
                if (!shader) {
                        this->shader = new Shader(v_path, f_path, g_path);
                }
        }

        virtual glm::mat4 get_model_matrix (void) = 0;
public:
        /* Рисование */
        virtual void draw    (const glm::mat4 &view, const glm::mat4 &projection) = 0;
        virtual void replace (GLfloat delta_time) {
                point = point + speed * delta_time * direction;
        }

        /* Геттеры */
        glm::vec3 get_point     (void) {
                return point;
        }
        glm::vec3 get_direction (void) {
                return direction;
        }
        GLfloat   get_speed     (void) {
                return speed;
        }

        virtual ~Object (void)
        {}
};

#endif