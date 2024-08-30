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

    void cleanup_gl() {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
    }

};

class PhongModel
{
  public:
    PhongModel() = delete;
    // initialize a phong model, loads up the model and populate all the meshes,
    // copying mesh vertex&index buffers onto device.
    PhongModel(const std::string& model_path);
    ~PhongModel();

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
