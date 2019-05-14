#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include <vector>

#include "glm/glm.hpp"

#include "shader.h"

class Mesh
{
public:
        struct Vertex
        {
                glm::vec3 position;
                glm::vec3 normal;
                glm::vec2 tex_coords;
                // tangent
                glm::vec3 tangent;
                // bitangent
                glm::vec3 bitangent;
        };

        struct Texture
        {
                GLuint id;
                std::string type;
                std::string path;
        };

        std::vector <Vertex> vertices;
        std::vector <GLuint> indeces;
        std::vector <Texture> textures;
        GLuint vao;

        Mesh (const std::vector<Vertex> &vertices, const std::vector<GLuint> &indeces, const std::vector<Texture> &textures);

        void draw(Shader shader);
private:
        GLuint vbo, ebo;
};

#endif