#include <fstream>
#include <iostream>
#include <sstream>

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
        int  success;
        char infoLog[512];
        glGetShaderiv(this->shader_id, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(this->shader_id, 512, NULL, infoLog);
            std::cout << "Shader compilation failed: " << infoLog << std::endl;
            return false;
        }
    }

    return true;
}
