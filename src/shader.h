#pragma once
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

      private:
        std::string source_file;
        unsigned int shader_kind;
        unsigned int shader_id = 0;
};
