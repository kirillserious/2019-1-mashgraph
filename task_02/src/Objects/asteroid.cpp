#include "asteroid.h"
#include "../fs.h"

const GLchar * Asteroid::model_path = "../models/asteroid/asteroid.obj";
const glm::vec3 Asteroid :: direction = glm::vec3(0.0f, 0.0f, 1.0f);
Model * Asteroid::model = nullptr;


Asteroid :: Asteroid (const glm::vec3 &point, GLfloat size)
: MeshObject(point, direction, 5.0f, model, Fs::make(model_path).c_str()),
  size(size), time(0.0f)
{
        if (!model) {
                model = MeshObject :: model;
        }
}

glm::mat4
Asteroid :: get_model_matrix (void)
{
        glm::mat4 model(1.0f);
        
        model = glm::translate(model, point);
        model = glm::scale(model, glm::vec3(0.004*size));
        model = glm :: rotate(model, 0.3f*time, glm::vec3(1.0, 7.0, -2.0));
        return model;
}

bool
Asteroid :: is_intersect (const glm::vec3 &point, GLfloat radius)
{
        GLfloat dist = glm::length(get_point() - point);
        GLfloat min_dist = get_size() + radius;
        if (dist < 2.0f * min_dist) {
                return true;
        } else {
                return false;
        }
}