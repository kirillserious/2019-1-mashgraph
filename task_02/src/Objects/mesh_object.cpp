#include "mesh_object.h"
#include "../fs.h"
#include <iostream>

const GLchar * MeshObject::v_path = "../shaders/asteroid.vs";
const GLchar * MeshObject::f_path = "../shaders/asteroid.fs";
const GLchar * MeshObject::g_path = "../shaders/asteroid.gs";

Shader * MeshObject::shader = nullptr;

MeshObject :: MeshObject
(
        const glm::vec3 &point,
        const glm::vec3 &direction,
        GLfloat speed,
        Model *model,
        const GLchar *model_path
) :
        Object(point, direction, speed, shader, Fs::make(v_path).c_str(), Fs::make(f_path).c_str(), Fs::make(g_path).c_str()),
        model(model),
        explosion_time(0.0f)
{
        if (!shader) {
                shader = Object :: shader;
        }
        if (!model) {
                this->model  = new Model(model_path);
        }
}

void
MeshObject :: draw (const glm::mat4 &view, const glm::mat4 &projection)
{
        shader->use();
        shader->set_uniform("model", get_model_matrix());
        shader->set_uniform("view", view);
        shader->set_uniform("projection", projection);
        if (explosion_time > 0.0f) {
                shader->set_uniform("explosion_time", glfwGetTime() - explosion_time);
        } else {
                shader->set_uniform("explosion_time", 0);
        }
        model->draw(*shader);

}

void
MeshObject :: explode (void)
{
        if (explosion_time < 0.01f) {
                explosion_time = glfwGetTime();
        } else {
                std::cout << "Asteroid::explode():: Second explosion" << std::endl;
        }
}