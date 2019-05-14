#ifndef SPACESHIP_H_
#define SPACESHIP_H_

#include "mesh_object.h"

class Spaceship : public MeshObject
{
public:
        Spaceship (const glm::vec3 &point, GLfloat speed=5);

        virtual bool is_intersect (const glm::vec3 &point, GLfloat radius);
protected:
        virtual glm::mat4 get_model_matrix (void);

        static const GLchar *model_path;
        static Model *model;
        static const glm::vec3 direction;
};

#endif