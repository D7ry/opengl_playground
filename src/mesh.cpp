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

        glVertexAttribPointer(
            0, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(0)
        );
        glVertexAttribPointer(
            1,              // index
            3,              // size
            GL_FLOAT,       // type
            false,          // normalized
            sizeof(Vertex), // stride
            reinterpret_cast<void*>(offsetof(Vertex, normal))
        );
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

void PhongModel::draw(
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& proj,
    ShaderProgram& shader
) {
    shader.use();
    shader.set_uniform_mat4(U_MODEL, model);
    shader.set_uniform_mat4(U_VIEW, view);
    shader.set_uniform_mat4(U_PROJ, proj);

    // glActiveTexture(GL_TEXTURE0);
    for (PhongMesh& mesh : this->meshes) {
        // TODO: add texture processing
        // glBindTexture(GL_TEXTURE_2D, this->texture.id);
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    glBindVertexArray(0); // unbind VAO
    // glBindTexture(GL_TEXTURE_2D, 0);
}

// model loading functions
PhongMesh PhongModel::process_mesh(aiMesh* mesh, const aiScene* scene) {
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
        if (mesh->mTextureCoords[0]) {
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

    bool success = ret.init_gl(vertices, indices);
    if (!success) {
        ERROR("Failed to initialize openGL resource");
    }

    return ret;
};

void PhongModel::process_node(aiNode* node, const aiScene* scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.emplace_back(process_mesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
    }
}

PhongModel::PhongModel(const std::string& model_path) {
    DEBUG("Loading mesh from {}", model_path);
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        model_path, aiProcess_Triangulate | aiProcess_FlipUVs
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
        || !scene->mRootNode) {
        CRIT("Error loading mesh");
    }
    // recursive node process
    process_node(scene->mRootNode, scene);
}

PhongModel::~PhongModel() {
    for (auto& mesh : this->meshes) {
        mesh.cleanup_gl();
    }
}
