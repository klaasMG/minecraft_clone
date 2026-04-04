#ifndef MINECRAFT_CLONE_APPLICATION_H
#define MINECRAFT_CLONE_APPLICATION_H
#include "render.h"
#include "player.h"
#include "chunks.h"
#include <GLFW/glfw3.h>
#include <iostream>

class Application {
public:
    int init_app();
    void run_app();
    int cleanup_app();
private:
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    GLFWwindow* window;
    Player player;
    ChunkManager chunk_manager;
    Renderer renderer;
};

#endif //MINECRAFT_CLONE_APPLICATION_H