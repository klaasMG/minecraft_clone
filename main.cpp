#include "render.h"
#include "player.h"
#include "chunks.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    player->make_projection_matrix(90.0f, aspect, 0.1f, 100.0f);
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));

    static bool firstMouse = true;
    static float lastX = 0.0f;
    static float lastY = 0.0f;

    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
        return;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    float sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    player->change_yaw(xoffset);
    player->change_pitch(yoffset);

    if (player->pitch > 89.0f) player->pitch = 89.0f;
    if (player->pitch < -89.0f) player->pitch = -89.0f;
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Quad", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return -1;
    }

    Renderer renderer;
    renderer.renderer_init();
    ChunkManager chunk_manager;
    Player player = Player();
    chunk_manager.get_chunks(glm::i64vec2(player.position[0], player.position[1]), 0);

    glfwSetWindowUserPointer(window, &player);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            player.move_forward(1);
            std::cout << "w" <<std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            player.move_forward(-1);
            std::cout << "s" <<std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            player.move_right(1);
            std::cout << "a" <<std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            player.move_right(-1);
            std::cout << "d" <<std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            player.move_up(1);
            std::cout << "space" <<std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            player.move_up(-1);
            std::cout << "shift" <<std::endl;
        }
        auto chunk_render_data = chunk_manager.get_chunk_render_date(glm::i64vec2(player.position[0], player.position[2]), 2);
        auto meshes = chunk_manager.get_meshes(glm::i64vec2(player.position[0], player.position[2]), 2);
        renderer.render(player.view, player.projection, chunk_render_data, meshes);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    renderer.renderer_destroy();

    glfwTerminate();
    return 0;
}