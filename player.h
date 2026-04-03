#ifndef MINECRAFT_CLONE_PLAYER_H
#define MINECRAFT_CLONE_PLAYER_H

#include "mat4x4.hpp"
#include "vec3.hpp"

class Player {
public:

private:
    void computeForward(float yaw, float pitch);
    void make_view_matrix();
    float yaw = 0;
    float pitch = 0;
    glm::vec3 postion = glm::vec3(0);
    glm::mat4x4 view = glm::mat4x4(0);
    glm::mat4x4 projection = glm::mat4x4(0);
    glm::vec3 center = glm::vec3(0);
    glm::vec3 forward = glm::vec3(0);
};

#endif //MINECRAFT_CLONE_PLAYER_H