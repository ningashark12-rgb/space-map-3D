#include "Renderer.h"

Renderer::Renderer() {
}

void Renderer::init() {
    // Enable 3D depth testing (so objects in front hide objects in back)
    glEnable(GL_DEPTH_TEST);
    
    // Enable blending for transparency (used for glowing paths)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set a dark space background
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
}

void Renderer::render(const std::vector<Star*>& stars, 
                      const std::vector<Star*>& path, 
                      const Camera& camera) {
    // 1. Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. Set up the camera matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = 1280.0f / 720.0f; // Assuming 1280x720 window
    glFrustum(-aspect * 0.1f, aspect * 0.1f, -0.1f, 0.1f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Apply the camera's view matrix
    glm::mat4 view = camera.getViewMatrix();
    glLoadMatrixf(&view[0][0]);

    // 3. Draw the stars as small dots
    glPointSize(2.0f);
    glColor3f(0.8f, 0.8f, 1.0f); // Soft blue-white
    glBegin(GL_POINTS);
    for (Star* star : stars) {
        glm::vec3 pos = star->getPosition();
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();

    // 4. Draw the path as a green line
    if (path.size() > 1) {
        glLineWidth(2.0f);
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glBegin(GL_LINE_STRIP);
        for (Star* star : path) {
            glm::vec3 pos = star->getPosition();
            glVertex3f(pos.x, pos.y, pos.z);
        }
        glEnd();
    }
}