#include <iostream>
#include <math.h>
#include <exception>
#include <unordered_set>

#include <stdlib.h>
#include <time.h> 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLT_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "gltext.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "window.h"
#include "camera.h"
#include "skybox.h"

#include "Objects/object.h"
#include "Objects/mesh_object.h"
#include "Objects/asteroid.h"
#include "Objects/laser.h"
#include "Objects/spaceship.h"

#include "game_frame.h"

#include "fs.h"

/* Камера */
Camera camera;

GLfloat deltaTime = 0.0f;    // Время, прошедшее между последним и текущим кадром
bool keys[1024];

void
do_movement()
{
  // Camera controls
  if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
  if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
  if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
  if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void
key_callback(GLFWwindow *window,
                    int key, 
                    int scancode, 
                    int action,
                    int mode)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if(action == GLFW_PRESS)
                keys[key] = true;
        else if(action == GLFW_RELEASE)
                keys[key] = false;
}

/* Мышь */
void
mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
        static GLfloat lastX = 400, lastY = 300;  // Типа центр экрана

        static bool firstMouse = true;        
        if (firstMouse) // эта переменная была проинициализирована значением true
        {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
        }
        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos; // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
}

GLfloat fov = 45.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
        if(fov >= 1.0f && fov <= 45.0f)
                fov -= yoffset;
        if(fov <= 1.0f)
                fov = 1.0f;
        if(fov >= 45.0f)
                fov = 45.0f;
}


int create_laser = 0;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        create_laser++;
    }
}


void
draw_objects 
(
    std::unordered_set<Object*> &set,
    const glm::mat4 &view,
    const glm::mat4 &projection
)
{
    for (auto it = set.begin(); it != set.end(); ++it) {
        (*it)->draw(view, projection);
    }
}

void
replace_objects 
(
    std::unordered_set<Object*> &set,
    GLfloat delta_time
)
{
    for (auto it = set.begin(); it != set.end(); ++it) {
        (*it)->replace(delta_time);
    }
}

void
delete_objects
(
        std::unordered_set<Object*> &set
)
{
        for (auto it = set.begin(); it != set.end(); ) {
                glm::vec3 point = (*it)->get_point();
                if ((point.z > 5.0f) || (point.z < -100.0f)) {
                        delete *it;
                        it = set.erase(it);
                } else {
                        ++it;
                }
        }
}

void
explode_mesh_objects
(
        std::unordered_set<Object*> &lasers,
        std::unordered_set<Object*> &meshs,
        GameFrame &game_frame
)
{
        for (auto lit = lasers.begin(); lit != lasers.end(); ) {
                bool is_deleted = false;
                for (auto mit = meshs.begin(); mit != meshs.end(); ) {
                        MeshObject *mptr = dynamic_cast<MeshObject*> (*mit);
                        Laser      *lptr = dynamic_cast<Laser*>      (*lit);
                        if (!mptr || !lptr) {
                                std::cout << "Ошибка. Добавили не то в объекты." << std::endl;
                                return;
                        }
                        auto point  = lptr->get_point();
                        auto radius = lptr->radius;
                        if ((!mptr->is_exploded()) && mptr->is_intersect(point, radius)) {
                                delete lptr;
                                lit = lasers.erase(lit);
                                mptr->explode();
                                is_deleted = true;

                                auto spaceship = dynamic_cast<Spaceship *> (mptr);
                                if (spaceship && !game_frame.is_end) {
                                    game_frame.score++;
                                }

                                break;
                        }
                        ++mit;
                        
                }
                if (!is_deleted) {
                        ++lit;
                }
        }


}

void
create_red_lasers
(
        std::unordered_set<Object*> &spaceships,
        std::unordered_set<Object*> &lasers
)
{
        for (auto it = spaceships.begin(); it != spaceships.end(); ++it) {
                auto sptr = dynamic_cast<Spaceship*> (*it); 
                if (!sptr->is_exploded()){
                        glm::vec3 point = (*it)->get_point();
                        glm::vec3 dir   = glm::normalize(camera.Position - point);
                        point = point + 3.0f * dir; 
                        glm::vec3 color = glm::vec3(1.0, 0.0, 0.0);
                        Laser *laser = new Laser(point, dir, color, 15.0f);
                        lasers.insert(laser);
                }
        }
}

void
user_mesh_intersect
(
        std::unordered_set<Object*> &set,
        GLfloat damage,
        GameFrame &frame
)
{
        for (auto it = set.begin(); it != set.end(); ++it) {
                auto mptr = dynamic_cast<MeshObject*> (*it);
                if (!mptr->is_exploded() && mptr->is_intersect(camera.Position, 0.2f)) {
                        frame.health -= damage;
                        mptr->explode();
                }
        }
}

void
game (void)
{
        /* Невидимая мышка */
        glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);      
            
        /* Включаем проверку глубины */
        glEnable(GL_DEPTH_TEST);

        /* Загружаем Skybox */
        try {
                std::vector<std::string> faces = 
                        {
                        Fs::make("../img/skybox/right.jpg").c_str(),
                        Fs::make("../img/skybox/left.jpg").c_str(),
                        Fs::make("../img/skybox/top.jpg").c_str(),
                        Fs::make("../img/skybox/bottom.jpg").c_str(),
                        Fs::make("../img/skybox/front.jpg").c_str(),
                        Fs::make("../img/skybox/back.jpg").c_str()
                        };
                Skybox :: load (faces);
        } catch (std::exception &e) {
                std::cout << e.what() << std::endl;
        }

        /* Установка CallBack */
        glfwSetKeyCallback         (Window::window, key_callback);
        glfwSetCursorPosCallback   (Window::window, mouse_callback);
        glfwSetScrollCallback      (Window::window, scroll_callback);
        glfwSetMouseButtonCallback (Window::window, mouse_button_callback);


        /* Основной цикл */
        GLfloat lastFrame = 0.0f;       // Время вывода последнего кадра
        
        std::unordered_set <Object*>  lasers;
        std::unordered_set <Object*>  asteroids;
        std::unordered_set <Object*>  spaceships;

        GameFrame frame;
        GLfloat last_creation = 0.0f;
        int creation_count = 0;
        while(!glfwWindowShouldClose(Window::window)) {
                /* Ловим события */
                glfwPollEvents();

                /* Очищаем буфферы */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                /* Двигаем камеру */
                do_movement();

                /* Матрицы */
                glm::mat4 view       = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(fov), (GLfloat)Window::get_width()/(GLfloat)Window::get_height(), 0.1f, 100.0f);

                /* Рисуем */
                draw_objects(asteroids,  view, projection);
                draw_objects(lasers,     view, projection);
                draw_objects(spaceships, view, projection);


                explode_mesh_objects(lasers, asteroids, frame);
                explode_mesh_objects(lasers, spaceships, frame);

                /* Запускаем лазеры */
                if (create_laser > 0) {
                        glm::vec3 point = camera.Position;
                        glm::vec3 dir = camera.Front;
                        point += 3.0f * dir;
                        Laser *laser_ptr = new Laser(point, dir);
                        lasers.insert(laser_ptr);
                        create_laser--;
                }

                user_mesh_intersect(asteroids,  0.1f, frame);
                user_mesh_intersect(spaceships, 0.3f, frame);
                for (auto it = lasers.begin(); it != lasers.end(); ) {
                        auto lptr = dynamic_cast<Laser*> (*it);
                        GLfloat dist = glm::length(lptr->get_point() - camera.Position);
                        GLfloat min_dist = 0.2f + lptr->radius;
                        if (dist < 2.0f * min_dist) {
                                frame.health -= 0.2f;
                                delete lptr;
                                it = lasers.erase(it);
                        } else {
                                ++it;
                        }
                }

                /* Поворачиваем астероиды */
                for (auto it = asteroids.begin(); it != asteroids.end(); ++it) {
                        auto aptr = dynamic_cast<Asteroid*> (*it); 
                        aptr->add_time(deltaTime);
                }
                
                replace_objects(lasers,     deltaTime);
                replace_objects(spaceships, deltaTime);
                replace_objects(asteroids,  deltaTime);

                delete_objects(lasers);
                delete_objects(asteroids);
                delete_objects(spaceships);

                Skybox :: draw (view, projection);
                frame.reset_text();
                frame.draw();

                glfwSwapBuffers(Window::window);

                /* Меняем время */
                GLfloat currentFrame = glfwGetTime();
                if (currentFrame - last_creation > 3.0f) {
                    creation_count++;
                    last_creation = currentFrame;
                    /* Создали объекты */
                    if (creation_count % 4 == 0) {
                        auto pos = glm::vec3(rand() % 20 - 10, rand() % 20 - 10, -100.0f);
                        Spaceship *spaceship = new Spaceship(pos);
                        spaceships.insert(spaceship);
                    } else {
                        auto pos  = glm::vec3(rand() % 20 - 10, rand() % 20 - 10, -100.0f);
                        GLfloat size = GLfloat(rand() % 10 + 2) / 10;
                        Asteroid *asteroid = new Asteroid(pos, size);
                        asteroids.insert(asteroid);
                    }
                    auto pos  = glm::vec3(rand() % 100 - 50, rand() % 100 - 50, -100.0f);
                    GLfloat size = 1.0f;
                    Asteroid *asteroid = new Asteroid(pos, size);
                    asteroid->explode();
                    asteroids.insert(asteroid);
                    
                    create_red_lasers(spaceships, lasers);
                }

                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                if (frame.health <= 0.0f) {
                    frame.end_game();
                }
        }
}


int
main (int argc, char** argv)
{
        /* Инициализируем генератор случайных чисел */
        srand(time(NULL));

        Fs::init(argv[0]);
        /* Создание окна */
        try {
                Window::create_window("Mashgraph 2", 800, 800);
        } catch (std::exception &e) {
                std::cout << e.what() << std::endl;
                return -1;
        }
        gltInit();


        game();
      


        // Destroy glText
        gltTerminate();
        Window::delete_window();

        return 0;
}
