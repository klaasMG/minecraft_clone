#include "player.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>

void Player::move_forward(float amount) {
    position += forward * amount;
    make_view_matrix();
}

void Player::move_right(float amount) {
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
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

    std::cout << "View matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << view[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

void Player::make_projection_matrix(float fov, float aspect, float near, float far){
    projection = glm::perspective(glm::radians(fov), aspect, near, far);

    std::cout << "Projection matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << projection[j][i] << " ";
        }
        std::cout << std::endl;
    }
}