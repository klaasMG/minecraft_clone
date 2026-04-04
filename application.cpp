#include "application.h"
#include "chunks.h"
#include "player.h"

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

int Application::init_app() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Quad", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    player = Player();
    glfwSetWindowUserPointer(window, &player);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return -1;
    }

    renderer.renderer_init();
    chunk_manager.get_chunks(glm::i64vec2(player.position[0], player.position[2]), 0);

    return 0;
}

void Application::run_app() {
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            player.move_forward(1);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            player.move_forward(-1);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            player.move_right(1);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            player.move_right(-1);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            player.move_up(1);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            player.move_up(-1);
        }
        auto chunk_render_data = chunk_manager.get_chunk_render_date(glm::i64vec2(player.position[0], player.position[2]), 2);
        auto meshes = chunk_manager.get_meshes(glm::i64vec2(player.position[0], player.position[2]), 2);
        renderer.render(player.view, player.projection, chunk_render_data, meshes);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int Application::cleanup_app() {
    renderer.renderer_destroy();
    glfwTerminate();
    return 0;
}
