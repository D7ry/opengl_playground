#pragma once

struct TextureHandle
{
    unsigned int id = 0;
};

class TextureManager
{
  public:
    TextureManager();

    // get a texture handle. caller can assume O(1) runtime for repeated gets.
    TextureHandle get_texture(const char* texture_path);

    ~TextureManager();

  private:
    std::unordered_map<std::string, TextureHandle> textures;
};
