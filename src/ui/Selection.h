#pragma once
#include <vector>
#include "core/Star.h"
#include "render/Camera.h"

class Selection {
public:
    // Takes mouse coordinates (x, y) and the list of stars, 
    // returns the star that was clicked (or nullptr if none)
    Star* getStarAtScreenPos(int x, int y,
                             const std::vector<Star*>& stars,
                             const Camera& camera);
};