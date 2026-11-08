#include "Renderer.h"
#include <SFML/OpenGL.hpp> // Make sure OpenGL is included

Renderer::Renderer() {
}

void Renderer::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
}

void Renderer::render(const std::vector<Star*>& stars, 
                      const std::vector<Star*>& path, 
                      const Camera& camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = 1280.0f / 720.0f;
    glFrustum(-aspect * 0.1f, aspect * 0.1f, -0.1f, 0.1f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glm::mat4 view = camera.getViewMatrix();
    glLoadMatrixf(&view[0][0]);

    // 1. Draw all the stars (Small blue dots)
    glPointSize(2.0f);
    glColor3f(0.8f, 0.8f, 1.0f); 
    glBegin(GL_POINTS);
    for (Star* star : stars) {
        glm::vec3 pos = star->getPosition();
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();

    // 2. Highlight the start and end stars of the path in RED
    if (path.size() > 0) {
        glPointSize(10.0f);
        glColor3f(1.0f, 0.0f, 0.0f); // RED
        
        // Draw start star
        glm::vec3 startPos = path[0]->getPosition();
        glBegin(GL_POINTS);
        glVertex3f(startPos.x, startPos.y, startPos.z);
        glEnd();

        // Draw end star
        glm::vec3 endPos = path.back()->getPosition();
        glBegin(GL_POINTS);
        glVertex3f(endPos.x, endPos.y, endPos.z);
        glEnd();
    }

    // 3. Draw the path as a green line
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