#include "healthbar.h"

#include "../glm/glm.hpp"
#include "../fs.h"

GLfloat HealthBar :: vertices [] =
{
        0.0, 0.95, 0.0, 0.93, 1.6, 0.95,
        0.0, 0.93, 1.6, 0.95, 1.6, 0.93
};

GLfloat HealthBar :: frame_vertices [] =
{
        -0.1, 0.96, -0.1, 0.92,
        -0.1, 0.92, 1.7, 0.92,
        1.7, 0.92, 1.7, 0.96,
        1.7, 0.96, -0.1, 0.96
};

Shader *HealthBar :: shader = nullptr;
Vao *HealthBar::vao         = nullptr;
Vao *HealthBar::frame_vao   = nullptr;

int HealthBar :: count = 0;

HealthBar :: HealthBar (GLfloat health)
: health(health)
{
        if (!shader) {
                shader = new Shader(Fs::make(vshader_path), Fs::make(fshader_path));
        }
        if (!vao) {
                vao = new Vao(vertices, 6, 2);
                vao->set_location(0, 2, 0);
        }
        if (!frame_vao) {
                frame_vao = new Vao(frame_vertices, 8, 2);
                frame_vao->set_location(0, 2, 0);
        }
        count ++;
}

HealthBar :: ~HealthBar (void)
{
        count--;
        if (!count) {
                delete shader;
                shader = nullptr;
                delete vao;
                vao = nullptr;
                delete frame_vao;
                frame_vao = nullptr;
        }
}

void
HealthBar :: draw (void)
{
        shader->use();
        glm::mat4 model(1.0);
        model = glm :: translate (model, glm :: vec3 (-0.8, 0.0, 0.0));
        shader->set_uniform("model", model);
        shader->set_uniform("health_color", 1.0f, 1.0f, 1.0f);
        frame_vao->draw_as_lines();

        model = glm :: mat4(1.0);
        model = glm::scale(model, glm::vec3(health, 1.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.8, 0.0f, 0.0f));
        shader->set_uniform("model", model);
        if (health > 0.7) {
                shader->set_uniform("health_color", 0.0f, 1.0f, 0.0f);
        } else if (health > 0.35) {
                shader->set_uniform("health_color", 1.0f, 1.0f, 0.0f);
        } else {
                shader->set_uniform("health_color", 1.0f, 0.0f, 0.0f);
        }        
        vao->draw();
        
}