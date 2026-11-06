#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    
    // Returns the 3D view matrix (used by the Renderer to draw the scene)
    glm::mat4 getViewMatrix() const;
    
    // Returns where the camera is in the world
    glm::vec3 getPosition() const;
    
    // Move the camera relative to where it's looking
    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);

private:
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    float speed_;
};