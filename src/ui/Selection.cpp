#include "Selection.h"
#include <cstdlib>

Star* Selection::getStarAtScreenPos(int x, int y,
                                    const std::vector<Star*>& stars,
                                    const Camera& camera) {
    if (stars.empty()) return nullptr;
    
    // Pick a completely random star to test the distance
    int randomIndex = rand() % stars.size();
    return stars[randomIndex];
}