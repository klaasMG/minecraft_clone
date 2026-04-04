#include "player.h"
#include "render.h"
#include <glad/glad.h>
#include <iostream>

void Renderer::bind_ubo() {
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_DYNAMIC_DRAW);
}

void Renderer::update_ubo(const glm::mat4x4& proj, const glm::mat4x4& view) {
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &proj);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view);
}

GLuint Renderer::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error:\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint Renderer::createProgram(const char* vertex_shader_src, const char* fragment_shader_src) {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertex_shader_src);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader_src);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program link error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Renderer::renderer_init() {
    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
        // Front
         4.5f, -0.5f,  0.5f,
         5.5f, -0.5f,  0.5f,
         5.5f,  0.5f,  0.5f,

         5.5f,  0.5f,  0.5f,
         4.5f,  0.5f,  0.5f,
         4.5f, -0.5f,  0.5f,

        // Back
         4.5f, -0.5f, -0.5f,
         4.5f,  0.5f, -0.5f,
         5.5f,  0.5f, -0.5f,

         5.5f,  0.5f, -0.5f,
         5.5f, -0.5f, -0.5f,
         4.5f, -0.5f, -0.5f,

        // Left
         4.5f,  0.5f,  0.5f,
         4.5f,  0.5f, -0.5f,
         4.5f, -0.5f, -0.5f,

         4.5f, -0.5f, -0.5f,
         4.5f, -0.5f,  0.5f,
         4.5f,  0.5f,  0.5f,

        // Right
         5.5f,  0.5f,  0.5f,
         5.5f, -0.5f, -0.5f,
         5.5f,  0.5f, -0.5f,

         5.5f, -0.5f, -0.5f,
         5.5f,  0.5f,  0.5f,
         5.5f, -0.5f,  0.5f,

        // Top
         4.5f,  0.5f, -0.5f,
         4.5f,  0.5f,  0.5f,
         5.5f,  0.5f,  0.5f,

         5.5f,  0.5f,  0.5f,
         5.5f,  0.5f, -0.5f,
         4.5f,  0.5f, -0.5f,

        // Bottom
         4.5f, -0.5f, -0.5f,
         5.5f, -0.5f,  0.5f,
         4.5f, -0.5f,  0.5f,

         5.5f, -0.5f,  0.5f,
         4.5f, -0.5f, -0.5f,
         5.5f, -0.5f, -0.5f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shaderProgram = createProgram(vertexShaderSrc, fragmentShaderSrc);
    bind_ubo();
    GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, "Matrices");
    glUniformBlockBinding(shaderProgram, blockIndex, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
}

void Renderer::renderer_destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void Renderer::render(const glm::mat4x4& view, const glm::mat4& proj, const std::vector<ChunkRenderDate>& chunck_render_data, const std::vector<std::vector<glm::vec4>>& meshes) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update_ubo(proj, view);

    size_t total_vertices = 0;
    for (const auto& data : chunck_render_data) {
        total_vertices += data.num_vertices;
    }

    if (total_vertices > 0) {
        std::vector<glm::vec4> all_vertices;
        all_vertices.reserve(total_vertices);
        for (const auto& mesh : meshes) {
            all_vertices.insert(all_vertices.end(), mesh.begin(), mesh.end());
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, all_vertices.size() * sizeof(glm::vec4), all_vertices.data(), GL_STREAM_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");

    for (const auto& data : chunck_render_data) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &data.model_matrix[0][0]);
        glDrawArrays(GL_TRIANGLES, data.vertex_offset, data.num_vertices);
    }
    std::cout << "Renderd:" <<total_vertices << std::endl;
}