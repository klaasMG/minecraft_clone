#ifndef MINECRAFT_CLONE_RENDER_H
#define MINECRAFT_CLONE_RENDER_H

#include "glad/glad.h"
#include <string>

class Renderer{
public:
    void renderer_init();
    void renderer_destroy();
    void render();
private:
    GLuint compileShader(GLenum type, const char *source);
    GLuint createProgram(const char* vertex_shader_src, const char* fragment_shader_src);
    GLuint VBO, VAO;
    GLuint shaderProgram;
    const char* vertexShaderSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
    }
    )";

    const char* fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(0.2, 0.6, 0.9, 1.0);
    }
    )";
};

#endif //MINECRAFT_CLONE_RENDER_H