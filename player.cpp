#include "player.h"
#include <cmath>

void Player::move(float dx, float dy, float dz) {
    position += glm::vec3(dx, dy, dz);
    make_view_matrix();
}

void Player::change_pitch(float delta_pitch) {
    pitch += delta_pitch;
    make_view_matrix();
}

void Player::change_yaw(float delta_yaw) {
    yaw += delta_yaw;
    make_view_matrix();
}

void Player::computeForward(){
    forward.x = std::cos(yaw) * std::cos(pitch);
    forward.y = std::sin(pitch);
    forward.z = std::sin(yaw) * std::cos(pitch);
    forward = glm::normalize(forward);
}

void Player::make_view_matrix(){
    glm::vec3 eye = position;
    glm::vec3 world_up = glm::vec3(0, 1, 0);
    glm::vec3 f = glm::normalize(forward);
    glm::vec3 r = glm::normalize(glm::cross(f, world_up));
    glm::vec3 u = glm::cross(r, f);

    view[0][0] = r.x;
    view[1][0] = r.y;
    view[2][0] = r.z;

    view[0][1] = u.x;
    view[1][1] = u.y;
    view[2][1] = u.z;

    view[0][2] = -f.x;
    view[1][2] = -f.y;
    view[2][2] = -f.z;

    view[3][0] = -glm::dot(r, eye);
    view[3][1] = -glm::dot(u, eye);
    view[3][2] =  glm::dot(f, eye);
}