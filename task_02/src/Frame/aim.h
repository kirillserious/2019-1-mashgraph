#ifndef AIM_H_
#define AIM_H_

#include "../shader.h"
#include "../vao.h"

/**
        Прицел на экране.
*/
class Aim
{
        static GLfloat vertices[];
        static Shader *shader;
        static Vao    *vao;
public:
        Aim();
        ~Aim();

        /// Рисует прицел. 
        void draw(void);
};

#endif