#ifndef MINECRAFT_CLONE_PLAYER_H
#define MINECRAFT_CLONE_PLAYER_H

#include "mat4x4.hpp"
#include "vec3.hpp"

struct Settings {
    int render_distance;
};

class Player {
public:
    Player();
    void change_yaw(float delta_yaw);
    void change_pitch(float delta_pitch);
    void move_forward(float amount);
    void move_right(float amount);
    void move_up(float amount);
    void make_projection_matrix(float fov, float aspect, float near, float far);
    void computeForward();
    void make_view_matrix();
    Settings settings = {.render_distance = 16};
    float pitch = 0;
    float yaw = 0;
    glm::mat4x4 view = glm::mat4x4(0);
    glm::mat4x4 projection = glm::mat4x4(0);
    glm::vec3 position{};

private:
    float velocity = 5;
    glm::vec3 center = glm::vec3(0);
    glm::vec3 forward = glm::vec3(0);
};

#endif //MINECRAFT_CLONE_PLAYER_H