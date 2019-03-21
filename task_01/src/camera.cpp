#include "camera.h"

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Camera::Camera(glm::vec3 position, GLfloat yaw, GLfloat pitch) 
: movement_speed(SPEED), mouse_sensitivity(SENSITIVTY)
{
    this->position = position;
    this->yaw = yaw;
    this->pitch = pitch;
}

glm::mat4
Camera :: get_transform ()
{
    glm::mat4 result(1.0f);
    result = glm::rotate(result, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec4 rotated_x = glm::rotate(result, -glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    result = glm::rotate(result, glm::radians(-pitch), glm::vec3(rotated_x));
    return result; 

}

glm::vec3
Camera :: get_position () 
{
    return this->position;
}

void
Camera :: process_mouse_movement (GLfloat   xoffset,
                                  GLfloat   yoffset,
                                  GLboolean constrain_pitch)
{
    xoffset *= this->mouse_sensitivity;
    yoffset *= this->mouse_sensitivity;

    this->yaw   += xoffset;
    this->pitch += yoffset;

    if (constrain_pitch) {
        if (this->pitch >  40.0f) {
            this->pitch =  40.0f;
        }
        if (this->pitch < -40.0f) {
            this->pitch = -40.0f;
        }
    }
}