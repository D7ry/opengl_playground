#pragma once

#include "assimp/scene.h"
class ShaderProgram;

class Material
{
};

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords; // texture coordinates
};

struct TextureHandle
{
    unsigned int id;
};

// Phong Mesh that lives in GPU memory
struct PhongMesh
{
    unsigned int VAO, VBO, EBO;
    TextureHandle texture;
    size_t num_indices;
    
    // initialize openGL resources from `vertices` and `indices`
    // returns whether initialization is successful
    bool init_gl(
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices
    );
};

class PhongModel
{
  public:
    PhongModel() = delete;
    PhongModel(const std::string& model_path);

    void draw(
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& proj,
        ShaderProgram& shader
    );

  private:
    PhongMesh process_mesh(aiMesh* mesh, const aiScene* scene);
    void process_node(aiNode* node, const aiScene* scene);
    std::vector<PhongMesh> meshes;
};
