#include "Camera.h"

Camera::Camera() {
    // Start at position (0, 0, 5) looking towards the origin
    position_ = glm::vec3(0.0f, 0.0f, 5.0f);
    
    // Looking in the negative Z direction
    front_ = glm::vec3(0.0f, 0.0f, -1.0f);
    
    // Up vector (Y axis)
    up_ = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Right vector is the cross product of front and up
    right_ = glm::normalize(glm::cross(front_, up_));
    
    speed_ = 5.0f;
}

glm::mat4 Camera::getViewMatrix() const {
    // Look from our position, in the direction we are facing
    return glm::lookAt(position_, position_ + front_, up_);
}

glm::vec3 Camera::getPosition() const {
    return position_;
}

void Camera::moveForward(float amount) {
    position_ += front_ * (amount * speed_);
}

void Camera::moveRight(float amount) {
    position_ += right_ * (amount * speed_);
}

void Camera::moveUp(float amount) {
    position_ += up_ * (amount * speed_);
}