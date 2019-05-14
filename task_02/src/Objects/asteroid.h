#ifndef ASTEROID_H_
#define ASTEROID_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../glm/glm.hpp"

#include "mesh_object.h"
#include "../shader.h"
#include "../model.h"

class Asteroid : public MeshObject
{
public:
        Asteroid (const glm::vec3 &point, GLfloat size);
        
        virtual bool is_intersect (const glm::vec3 &point, GLfloat radius);

        void
        add_time (GLfloat delta_time)
        {
                time += delta_time;
        }

        GLfloat get_size (void) {
                return size;
        }
protected:
        static const GLchar *model_path;
        static Model *model;
        virtual glm::mat4 get_model_matrix(void);
        GLfloat size;
        GLfloat time;
        static const glm::vec3 direction;
}; 

#endif