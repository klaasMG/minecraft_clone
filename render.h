#ifndef MINECRAFT_CLONE_RENDER_H
#define MINECRAFT_CLONE_RENDER_H

#include "glad/glad.h"
#include "mat4x4.hpp"
#include "chunks.h"
#include "mutex"
#include "player.h"
#include <atomic>

class Renderer{
public:
    void renderer_init();
    void renderer_destroy();
    void render(const glm::mat4x4& view, const glm::mat4x4& proj, const std::vector<ChunkRenderData>& chunk_render_data, const std::vector<std::vector<glm::vec4>>& meshes);
private:
    void bind_ubo();
    void update_ubo(const glm::mat4x4& proj, const glm::mat4x4& view);
    GLuint compileShader(GLenum type, const char *source);
    GLuint createProgram(const char* vertex_shader_src, const char* fragment_shader_src);
    GLuint UBO = 0;
    GLuint VBO = 0;
    GLuint VAO = 0;
    GLuint shaderProgram = 0;
    const char* vertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec4 aPos;
        layout(std140) uniform Matrices{
            mat4 proj;
            mat4 view;
        };
        uniform mat4 model;

        void main(){
            gl_Position = proj * view * model * aPos;
        })";

    const char* fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;

    void main(){
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    })";
};

#endif //MINECRAFT_CLONE_RENDER_H