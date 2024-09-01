#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "shared.h"
#include "transform.h"
#include "ecs/entity.h"

#include "system.h"

void PhongRenderSystem::tick(const TickData* tick_data) {
    shader->use();
    shader->set_uniform_mat4(U_VIEW, tick_data->camera->get_view_matrix());
    shader->set_uniform_mat4(U_PROJ, tick_data->proj);
    shader->set_uniform_vec3("u_cam_pos", tick_data->camera->get_position());

    // set position color intensity of the only light source
    // TODO: enable custom light sources
    shader->set_uniform_vec3("u_light_position", {4, 1, 1});
    shader->set_uniform_vec3("u_light_color", {1, 1, 1});
    shader->set_uniform_float("u_light_intensity", 5);

    // set up texture offsets
    shader->set_uniform_int("t_diffuse", 0);
    shader->set_uniform_int("t_specular", 1);
    shader->set_uniform_int("t_ambient", 2);
    // TODO: enable height mapping

    for (Entity* entity: this->entities) {
        auto transform = entity->get_component<Transform>();
        auto model = entity->get_component<PhongModel>();
        // phong model rendering logic
        shader->set_uniform_mat4(U_MODEL, transform->model());
        for (PhongMesh& mesh : model->meshes) {
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

            // unbind all textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

PhongRenderSystem::PhongRenderSystem() {
    this->shader = new SimpleShaderProgram(
        "../shaders/phong_mesh.vert", "../shaders/phong_mesh.frag"
    );
    shader->build();
}

void EntityViewerGUISystem::tick(const TickData* tick_data) {
    for (Entity* entity : this->entities) {
        Transform* transform = entity->get_component<Transform>();
        if (transform) {
            ImGui::SliderFloat("Scale x", &transform->scale.x, 0, 10);
            ImGui::SliderFloat("Scale y", &transform->scale.y, 0, 10);
            ImGui::SliderFloat("Scale z", &transform->scale.z, 0, 10);
        }
    }
}
