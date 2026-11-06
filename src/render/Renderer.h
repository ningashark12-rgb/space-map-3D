#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include "Camera.h"
#include "core/Star.h" // We include the Mock core

class Renderer {
public:
    Renderer();
    
    // Initial setup (called once)
    void init();
    
    // Draw the stars and the path
    void render(const std::vector<Star*>& stars, 
                const std::vector<Star*>& path, 
                const Camera& camera);

private:
    // Helper function to draw lines
    void drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);
};