#include "spaceship.h"
#include "../fs.h"
const GLchar * Spaceship::model_path = "../models/spaceship/E 45 Aircraft_obj.obj";
const glm::vec3 Spaceship :: direction = glm::vec3(0.0f, 0.0f, 1.0f);
Model * Spaceship::model = nullptr;

Spaceship :: Spaceship (const glm::vec3 &point, GLfloat speed)
: MeshObject(point, direction, speed, model, Fs::make(model_path).c_str())
{
        model = MeshObject :: model;
}

glm::mat4
Spaceship :: get_model_matrix (void)
{
        glm::mat4 model(1.0f);
        
        model = glm::translate(model, point);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));

        return model;
}

bool
Spaceship :: is_intersect (const glm::vec3 &point, GLfloat radius)
{
        GLfloat dist = glm::length(get_point() - point);
        GLfloat min_dist = 1.0f + radius;
        if (dist < 2.0f * min_dist) {
                return true;
        } else {
                return false;
        }
}