#ifndef MESH_OBJECT_H_
#define MESH_OBJECT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../glm/glm.hpp"

#include "object.h"
#include "../shader.h"
#include "../model.h"

class MeshObject : public Object
{
public :
        GLfloat explosion_time;

        MeshObject
        (
                const glm::vec3 &point,
                const glm::vec3 &direction,
                GLfloat speed,
                Model *model,
                const GLchar *model_path
        );

        virtual void  explode (void);        
        virtual void  draw(const glm::mat4 &view, const glm::mat4 &projection);
        virtual bool  is_intersect (const glm::vec3 &point, GLfloat radius) = 0;
        
        virtual bool  is_exploded (void) {
                return explosion_time > 0.0f;
        }
protected:
        static const GLchar *v_path;
        static const GLchar *f_path;
        static const GLchar *g_path;

        static Shader *shader;
        Model  *model;

        virtual glm::mat4 get_model_matrix(void) = 0;

};

#endif