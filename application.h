#ifndef MINECRAFT_CLONE_APPLICATION_H
#define MINECRAFT_CLONE_APPLICATION_H
#include "render.h"
#include "player.h"
#include "chunks.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

class Application {
public:
    int init_app();
    void run_app();
    int cleanup_app();
private:
    GLFWwindow* window;
    Player player;
    ChunkManager chunk_manager;
    Renderer renderer;
    std::thread render_thread;
    bool running = true;
};

#endif //MINECRAFT_CLONE_APPLICATION_H