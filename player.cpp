#include "player.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>

Player::Player() {
    position = glm::vec3(0.0f, 256.0f, 0.0f);
    pitch = 0.0f;
    make_projection_matrix(90.0f, 1.0f, 0.1f, 100.0f);
    computeForward();
    make_view_matrix();
}

void Player::move_forward(float amount) {
    glm::vec3 flatForward = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
    position += flatForward * amount;
    make_view_matrix();
}

void Player::move_right(float amount) {
    glm::vec3 flatForward = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
    glm::vec3 right = glm::normalize(glm::cross(flatForward, glm::vec3(0, 1, 0)));
    position += right * amount;
    make_view_matrix();
}

void Player::move_up(float amount) {
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    glm::vec3 up = glm::normalize(glm::cross(right, forward));
    position += up * amount;
    make_view_matrix();
}

void Player::change_pitch(float delta_pitch) {
    pitch += delta_pitch;
    computeForward();
    make_view_matrix();
}

void Player::change_yaw(float delta_yaw) {
    yaw += delta_yaw;
    computeForward();
    make_view_matrix();
}

void Player::computeForward(){
    forward.x = std::cos(yaw) * std::cos(pitch);
    forward.y = std::sin(pitch);
    forward.z = std::sin(yaw) * std::cos(pitch);
    forward = glm::normalize(forward);
}

void Player::make_view_matrix(){
    glm::vec3 target = position + forward;
    view = glm::lookAt(position, target, glm::vec3(0, 1, 0));
}

void Player::make_projection_matrix(float fov, float aspect, float near, float far){
    projection = glm::perspective(glm::radians(fov), aspect, near, far);
}