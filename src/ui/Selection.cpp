#include "Selection.h"
#include <glm/glm.hpp>
#include <limits>
#include <cmath>

Star* Selection::getStarAtScreenPos(int x, int y,
                                    const std::vector<Star*>& stars,
                                    const Camera& camera) {
    Star* closestStar = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    float screenWidth = 1280.0f;
    float screenHeight = 720.0f;
    
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);

    for (Star* star : stars) {
        glm::vec3 pos = star->getPosition();
        glm::vec4 clipSpace = projection * view * glm::vec4(pos, 1.0f);
        
        if (clipSpace.w <= 0.0f) continue;

        glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;
        float screenX = (ndc.x + 1.0f) * 0.5f * screenWidth;
        float screenY = (1.0f - ndc.y) * 0.5f * screenHeight;

        float dx = screenX - x;
        float dy = screenY - y;
        float distanceToMouse = std::sqrt(dx*dx + dy*dy);

        if (distanceToMouse < 100.0f && distanceToMouse < minDistance) {
            minDistance = distanceToMouse;
            closestStar = star;
        }
    }

    return closestStar;
}