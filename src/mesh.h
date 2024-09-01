#pragma once

#include "assimp/scene.h"
#include "texture.h"
#include "ecs/component.h"

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

// Phong Mesh that lives in GPU memory
struct PhongMesh
{
    unsigned int VAO, VBO, EBO;
    size_t num_indices;

    // initialize openGL resources from `vertices` and `indices`
    // returns whether initialization is successful
    bool init_gl(
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices
    );

    void cleanup_gl();

    // note here we only load one texture for each type
    // realistically one model can have multiple maps but i dont care

    TextureHandle tex_diffuse;  // diffuse map
    TextureHandle tex_specular; // specular map
    TextureHandle tex_ambient;  // ambient map
    TextureHandle tex_height;   // height map
};

class PhongModel : public Component
{
  public:
    PhongModel() = delete;
    // initialize a phong model, loads up the model and populate all the meshes,
    // copying mesh vertex&index buffers onto device.
    PhongModel(const std::string& model_path, TextureManager* texture_manager);
    ~PhongModel();

    void draw(
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& proj,
        const glm::vec3& cam_pos,
        ShaderProgram& shader
    );

    std::vector<PhongMesh> meshes;

  private:
    PhongMesh process_mesh(
        aiMesh* mesh,
        const aiScene* scene,
        TextureManager* texture_manager
    );
    void process_node(
        aiNode* node,
        const aiScene* scene,
        TextureManager* texture_manager
    );
    std::string model_path; // path to the model
};
