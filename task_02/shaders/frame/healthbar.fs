#version 330 core

out vec4 color;

uniform vec3 health_color;

void
main ()
{
    color = vec4(health_color, 1.);
}