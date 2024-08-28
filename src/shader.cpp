#include <fstream> // reading shader file
#include <sstream> // reading shader file

#include "spdlog/spdlog.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
bool Shader::compile() {
    std::ifstream is(this->source_file);
    if (!is.is_open()) {
        return false;
    }
    std::stringstream ss;
    ss << is.rdbuf();

    std::string shader_source = ss.str();
    const char* shader_source_ptr = shader_source.c_str();

    this->shader_id = glCreateShader(this->shader_kind);
    glShaderSource(
        this->shader_id,
        1,                  // count
        &shader_source_ptr, // ptr to char* of shader source
        NULL                // length
    );

    glCompileShader(this->shader_id);

    { // check shader compilation status
        int success;
        char infoLog[512];
        glGetShaderiv(this->shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(this->shader_id, 512, NULL, infoLog);
            ERROR("Shader compilation failed : {}", std::string(infoLog));
            return false;
        }
    }

    return true;
}

SimpleShaderProgram::SimpleShaderProgram(
    const std::string& vertex_shader_file,
    const std::string& fragment_shader_file
) {
    this->vertex_shader_file = vertex_shader_file;
    this->fragment_shader_file = fragment_shader_file;
}

bool SimpleShaderProgram::build() {
    // compile single shaders
    INFO(
        "Compiling shaders...\n    Vertex: {}\n    Fragment: {}",
        vertex_shader_file,
        fragment_shader_file
    );
    Shader vert_shader = Shader(vertex_shader_file.c_str(), GL_VERTEX_SHADER);
    Shader frag_shader
        = Shader(fragment_shader_file.c_str(), GL_FRAGMENT_SHADER);

    bool success = vert_shader.compile() && frag_shader.compile();

    if (!success) {
        ERROR("Error: shader compilation failed");
        return false;
    }

    this->program_id = glCreateProgram();
    // link shader program
    glAttachShader(this->program_id, vert_shader.shader_id);
    glAttachShader(this->program_id, frag_shader.shader_id);
    glLinkProgram(this->program_id);
    INFO("...success");

    return true;
}

void SimpleShaderProgram::use() { glUseProgram(this->program_id); }

void ShaderProgram::set_uniform_bool(
    const std::string& name,
    bool value
) {
    glUniform1i(
        glGetUniformLocation(this->program_id, name.c_str()), (int)value
    );
}

void ShaderProgram::set_uniform_int(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(this->program_id, name.c_str()), value);
}

void ShaderProgram::set_uniform_float(
    const std::string& name,
    float value
) {
    glUniform1i(glGetUniformLocation(this->program_id, name.c_str()), value);
}

void ShaderProgram::set_uniform_mat4(
    const std::string& name,
    const glm::mat4& mat4
) {
    int uniform_id = glGetUniformLocation(this->program_id, name.c_str());
    glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(mat4));
}
