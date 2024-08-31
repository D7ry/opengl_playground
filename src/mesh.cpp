#include "mesh.h"
#include "shader.h"
#include "stb_image.h"

// for model loading
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

bool PhongMesh::init_gl(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices
) {
    // allocate resources
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    // set up resources for this->VAO
    //
    // layout (location = 0) in vec3 a_pos;
    // layout (location = 1) in vec3 a_normal;
    // layout (location = 2) in vec2 a_tex_coords;
    glBindVertexArray(this->VAO);
    {
        // vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(vertices[0]),
            vertices.data(),
            GL_STATIC_DRAW // update once, draw many times
        );

        glEnableVertexAttribArray(0); // position
        glEnableVertexAttribArray(1); // normal
        glEnableVertexAttribArray(2); // texture coordinates


        // layout (location = 0) in vec3 a_pos;
        glVertexAttribPointer(
            0, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(0)
        );

        // layout (location = 1) in vec3 a_normal;
        glVertexAttribPointer(
            1,              // index
            3,              // size
            GL_FLOAT,       // type
            false,          // normalized
            sizeof(Vertex), // stride
            reinterpret_cast<void*>(offsetof(Vertex, normal))
        );

        // layout (location = 2) in vec2 a_tex_coords;
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            false,
            sizeof(Vertex),
            reinterpret_cast<void*>(offsetof(Vertex, tex_coords))
        );

        // index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(indices[0]),
            indices.data(),
            GL_STATIC_DRAW // update once, draw many times
        );
    }
    glBindVertexArray(0);

    this->num_indices = indices.size();

    // TODO: return proper result value from opengl call
    return true;
}

void PhongMesh::cleanup_gl() {
    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (VBO)
        glDeleteBuffers(1, &VBO);
    if (EBO)
        glDeleteBuffers(1, &EBO);
}

void PhongModel::draw(
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& proj,
    ShaderProgram& shader
) {
    shader.use();
    // set model view proj matrix, shared among all meshes of this model
    shader.set_uniform_mat4(U_MODEL, model);
    shader.set_uniform_mat4(U_VIEW, view);
    shader.set_uniform_mat4(U_PROJ, proj);

    for (PhongMesh& mesh : this->meshes) {
        // set up textures
        // 0 - diffuse
        // 1 - specular
        // 2 - ambient
        // 3 - height
        // FIXME: i don't like this
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.tex_diffuse.id);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mesh.tex_specular.id);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, mesh.tex_ambient.id);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, mesh.tex_height.id);

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // unbind all textures TODO: I don't like this ugh
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    glBindVertexArray(0); // unbind VAO
}

// model loading functions
PhongMesh PhongModel::process_mesh(
    aiMesh* mesh,
    const aiScene* scene, // parent scene owning the mesh
    TextureManager* texture_manager
) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    PhongMesh ret;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        char* p_vertex = reinterpret_cast<char*>(std::addressof(vertex));
        // copy over data
        static_assert(sizeof(Vertex::position) == sizeof(float) * 3);
        static_assert(sizeof(Vertex::normal) == sizeof(float) * 3);
        static_assert(sizeof(Vertex::tex_coords) == sizeof(float) * 2);

        memcpy(
            p_vertex + offsetof(Vertex, position),
            std::addressof(mesh->mVertices[i].x),
            sizeof(float) * 3
        );
        memcpy(
            p_vertex + offsetof(Vertex, normal),
            std::addressof(mesh->mNormals[i].x),
            sizeof(float) * 3
        );
        if (mesh->mTextureCoords[0]) { // only take 0th texture coordinate
            memcpy(
                p_vertex + offsetof(Vertex, tex_coords),
                &(mesh->mTextureCoords[0][i].x),
                sizeof(float) * 2
            );
        }

        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // populate model's textures
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    for (aiTextureType texture_type :
         {aiTextureType::aiTextureType_DIFFUSE,
          aiTextureType::aiTextureType_SPECULAR,
          aiTextureType::aiTextureType_HEIGHT,
          aiTextureType::aiTextureType_AMBIENT}) {
        aiString texture_name_;
        material->GetTexture(texture_type, 0, &texture_name_);
        std::string texture_name = std::string(texture_name_.C_Str());

        if (texture_name.empty()) {
            continue;
        }

        // figure out the abs pathing
        std::string texture_path = (this->model_path.substr(0, this->model_path.find_last_of('/') + 1) + texture_name).c_str();
        TextureHandle texture_handle
            = texture_manager->get_texture(texture_path.c_str());
        switch (texture_type) {
        case aiTextureType::aiTextureType_DIFFUSE:
            //DEBUG("Loaded defuse texture");
            ret.tex_diffuse = texture_handle;
            break;
        case aiTextureType::aiTextureType_SPECULAR:
            //DEBUG("Loaded specular texture");
            ret.tex_specular = texture_handle;
            break;
        case aiTextureType::aiTextureType_HEIGHT:
            //DEBUG("Loaded height texture");
            ret.tex_height = texture_handle;
            break;
        case aiTextureType::aiTextureType_AMBIENT:
            //DEBUG("Loaded ambient texture");
            ret.tex_ambient = texture_handle;
            break;
        default:
            CRIT("unreacheable statement");
        }
    }

    bool success = ret.init_gl(vertices, indices);
    if (!success) {
        ERROR("Failed to initialize openGL resource");
    }

    return ret;
};

void PhongModel::process_node(
    aiNode* node,
    const aiScene* scene,
    TextureManager* texture_manager
) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.emplace_back(process_mesh(mesh, scene, texture_manager));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene, texture_manager);
    }
}

PhongModel::PhongModel(
    const std::string& model_path,
    TextureManager* texture_manager
) {
    DEBUG("Loading mesh from {}", model_path);
    this->model_path = model_path;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        model_path, aiProcess_Triangulate | aiProcess_FlipUVs
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
        || !scene->mRootNode) {
        CRIT("Error loading mesh");
    }
    // recursive node process
    process_node(scene->mRootNode, scene, texture_manager);
}

PhongModel::~PhongModel() {
    for (auto& mesh : this->meshes) {
        mesh.cleanup_gl();
    }
}
