#include "laser.h"
#include "../fs.h"

const GLchar * Laser::v_path = "../shaders/laser.vs";
const GLchar * Laser::f_path = "../shaders/laser.fs";
const GLchar * Laser::t_path = "../img/face.jpg";


Shader    *Laser :: shader = nullptr;
Vao       *Laser :: vao    = nullptr;
Texture2D *Laser::texture = nullptr;

GLfloat Laser :: vertices[] = 
{   
    // Позиции            // Текстурные координаты
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,

    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,

    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,

    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f
};

Laser :: Laser
(
        const glm::vec3 &point,
        const glm::vec3 &direction,
        const glm::vec3 &color,
        GLfloat speed
) : Object(point, direction, speed, shader, Fs::make(v_path).c_str(), Fs::make(f_path).c_str()), color(color)
{
        if (!shader) {
                shader = Object::shader;
                vao = new Vao(vertices, 36, 3);
                vao->set_location(0, 3, 0);
                vao->set_location(1, 2, 3);
                texture = new Texture2D(Fs::make(t_path).c_str());
        }
        radius = 0.2f;
}

glm::mat4
Laser :: get_model_matrix (void)
{
        glm::mat4 model(1.0f);
        model = glm::translate(model, point);
        model = glm::scale(model, glm::vec3(radius, radius, radius));
        return model;
}

void
Laser :: draw (const glm::mat4 &view, const glm::mat4 &projection)
{
        shader->use();
        glm::mat4 model = get_model_matrix ();
        shader->set_uniform("model", model);
        shader->set_uniform("view", view);
        shader->set_uniform("projection", projection);
        shader->set_uniform("color", color);
        shader->set_uniform_texture("Texture", *texture, 1);
        vao->draw();
}