#include "stb_image.h"

#include "stb_image.h"
#include "texture.h"

TextureHandle TextureManager::get_texture(const char* texture_path) {
    std::string key = std::string(texture_path);
    auto it = this->textures.find(key);
    if (it != this->textures.end()) {
        return it->second;
    }

    DEBUG("Loading texture from {}", key);

    TextureHandle texture_handle;

    int width, height, number_components;
    unsigned char* data
        = stbi_load(texture_path, &width, &height, &number_components, 0);

    if (data == nullptr) {
        ERROR("Failed to load texture from {}", texture_path);
        return texture_handle;
    }

    GLenum format = 0;

    switch (number_components) {
    case 1:
        format = GL_RED;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        ERROR(
            "Invalid texture format. number_components == {}", number_components
        );
    }

    glGenTextures(1, &texture_handle.id);
    glBindTexture(GL_TEXTURE_2D, texture_handle.id);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    this->textures.insert({std::string(texture_path), texture_handle});

    stbi_image_free(data);

    return texture_handle;
}

TextureManager::TextureManager() { 
    // opengl's vertical tex coordinates are flipped, so flip it back at load time
    stbi_set_flip_vertically_on_load(true); 
}

TextureManager::~TextureManager() {
    for (auto it : this->textures) {
        glDeleteTextures(1, &(it.second.id));
    }
}
