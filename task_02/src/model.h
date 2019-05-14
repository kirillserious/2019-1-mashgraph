#ifndef MODEL_H_
#define MODEL_H_

#include <GL/glew.h> 

#include "glm/glm.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

#include <string>
#include <vector>

class Model
{
public:
        std::vector<Mesh::Texture> loaded_textures;
        std::vector<Mesh> meshes;
        std::string directory;

        Model (const std::string &path);
        void draw (Shader shader);
        static GLuint TextureFromFile(const char *path, const std::string &directory);
private:
        void  process_node  (aiNode *node, const aiScene *scene);
        Mesh  process_mesh  (aiMesh *mesh, const aiScene *scene);

        std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif