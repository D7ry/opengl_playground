#pragma once
#include "glm/glm.hpp"
#include <string>

class Shader
{
  public:
    Shader() = delete;

    Shader(const char* source_file, unsigned int type) {
        this->source_file = source_file;
        this->shader_kind = type;
    }

    ~Shader() {
        // free up resources
        if (this->shader_id != 0) {
            glDeleteShader(this->shader_id);
        }
    }

    // compile shader, return whether it's succeeded
    bool compile();

    unsigned int shader_id = 0;

  private:
    std::string source_file;
    unsigned int shader_kind;
};

class ShaderProgram
{
  public:
    virtual void use() { ERROR("Default use() not defined"); }

    void set_uniform_bool(const std::string& name, bool value);
    void set_uniform_int(const std::string& name, int value);
    void set_uniform_float(const std::string& name, float value);
    void set_uniform_mat4(const std::string& name, const glm::mat4& mat4);
    void set_uniform_vec3(const std::string& name, const glm::vec3& vec3);

    virtual bool build() {
        ERROR("Default build() not defined");
        return false;
    }

  protected:
    unsigned int program_id = 0;
};

// very simple shader program consisting only vertex and fragment shader
class SimpleShaderProgram : public ShaderProgram
{
  public:
    SimpleShaderProgram() = delete;

    // initialize a shader program, compiling and linking the shaders from the
    // provided files
    SimpleShaderProgram(
        const std::string& vertex_shader_file,
        const std::string& fragment_shader_file
    );

    ~SimpleShaderProgram() {
        if (this->program_id != 0) {
            glDeleteProgram(this->program_id);
        }
    }

    virtual bool build() override;

    // use the shaders. call this in render loop
    virtual void use() override;

  private:
    std::string vertex_shader_file;
    std::string fragment_shader_file;
};
