#ifndef MINECRAFT_CLONE_RENDER_H
#define MINECRAFT_CLONE_RENDER_H

#include "glad/glad.h"
#include "mat4x4.hpp"
#include "player.h"

class Renderer{
public:
    void renderer_init();
    void renderer_destroy();
    void render(const glm::mat4x4& view, const glm::mat4& proj, const std::vector<glm::mat4x4>& model_matricies, const std::vector<std::vector<glm::mat4x4>>& model_meshes);
private:
    void bind_ubo();
    void update_ubo(const glm::mat4x4& proj, const glm::mat4x4& view);
    GLuint compileShader(GLenum type, const char *source);
    GLuint createProgram(const char* vertex_shader_src, const char* fragment_shader_src);
    GLuint UBO;
    GLuint VBO, VAO;
    GLuint shaderProgram;
    const char* vertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout(std140) uniform Matrices{
            mat4 proj;
            mat4 view;
        };

        void main(){
            mat4 model = mat4(1.0);
            gl_Position = proj * view * model * vec4(aPos, 1.0);
        })";

    const char* fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;

    void main(){
        FragColor = vec4(0.2, 0.6, 0.9, 1.0);
    })";
};

#endif //MINECRAFT_CLONE_RENDER_H