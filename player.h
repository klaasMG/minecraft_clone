#ifndef MINECRAFT_CLONE_PLAYER_H
#define MINECRAFT_CLONE_PLAYER_H

#include "mat4x4.hpp"
#include "vec3.hpp"

class Player {
public:
    void change_yaw(float delta_yaw);
    void change_pitch(float delta_pitch);
    void move(float dx, float dy, float dz);
    float pitch = 0;
    float yaw = 0;
private:
    void computeForward();
    void make_view_matrix();
    glm::vec3 position = glm::vec3(0);
    glm::mat4x4 view = glm::mat4x4(0);
    glm::mat4x4 projection = glm::mat4x4(0);
    glm::vec3 center = glm::vec3(0);
    glm::vec3 forward = glm::vec3(0);
};

#endif //MINECRAFT_CLONE_PLAYER_H