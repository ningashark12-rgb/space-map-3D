#include "Selection.h"
#include <cmath>

Star* Selection::getStarAtScreenPos(int x, int y,
                                    const std::vector<Star*>& stars,
                                    const Camera& camera) {
    // This is a simplified mock version of mouse picking.
    // In a full 3D engine, we would cast a ray from the camera through the mouse.
    // For now, we will just return the closest star to the camera to prove it works.
    
    Star* closest = nullptr;
    float minDist = 99999.0f;
    
    for (Star* star : stars) {
        // Simple check: find the star closest to the camera position
        float dist = glm::length(star->getPosition() - camera.getPosition());
        if (dist < minDist) {
            minDist = dist;
            closest = star;
        }
    }
    
    return closest;
}