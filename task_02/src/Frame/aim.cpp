#include "aim.h"
#include "../fs.h"

GLfloat Aim::vertices[] =
{
         0.15,  0.0,  0.25,  0.0,
        -0.15,  0.0, -0.25,  0.0,
         0.0,   0.15, 0.0, 0.25,
         0.0, -0.15, 0.0, -0.25
};

Shader *Aim::shader = nullptr;
Vao    *Aim::vao    = nullptr;

Aim::Aim ()
{
        if (!shader) {
                shader = new Shader(Fs::make("../shaders/aim.vs"), Fs::make("../shaders/aim.fs"));
        }
        if (!vao) {
                vao = new Vao(vertices, 8, 2);
                vao->set_location(0, 2, 0);
        }
}

Aim::~Aim()
{}

void
Aim::draw(void)
{
        shader->use();
        vao->draw_as_lines();
}