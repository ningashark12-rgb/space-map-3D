#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "core/StarMap.h"
#include "pathfinding/AStarPathfinder.h"
#include "pathfinding/Heuristic.h"
#include "render/Camera.h"
#include "render/Renderer.h"

int main() {
    // 1. Create a 1280x720 window (SFML 3.0 syntax)
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "3D Space Map");
    window.setVerticalSyncEnabled(true);

    // 2. Initialize Renderer and Camera
    Renderer renderer;
    renderer.init();
    Camera camera;

        // 3. Generate a random galaxy of 500 stars
    StarMap map;
    for (int i = 0; i < 500; i++) {
        float x = (rand() % 100 - 50) / 5.0f;
        float y = (rand() % 100 - 50) / 5.0f;
        float z = (rand() % 100 - 150) / 5.0f;
        map.addStar(new Star("Star", glm::vec3(x, y, z), 1.0f, StarType::MainSequence));
    }

    // Pick two stars to find a path between
    Star* starA = map.getAllStars()[0];
    Star* starB = map.getAllStars()[499];
    map.addRoute(starA, starB);
    
    // 4. Run A* to find a path
    auto heuristic = std::make_unique<EuclideanHeuristic>();
    AStarPathfinder astar(std::move(heuristic));
    std::vector<Star*> path = astar.findPath(&map, starA, starC);

    // 5. Main Game Loop
    while (window.isOpen()) {
        // SFML 3.0 event loop
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Handle WASD movement (SFML 3.0 uses Key:: instead of just the letter)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) camera.moveForward(0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) camera.moveForward(-0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) camera.moveRight(-0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) camera.moveRight(0.1f);

        // Render everything
        renderer.render(map.getAllStars(), path, camera);

        // Display the window
        window.display();
    }

    // Cleanup
    delete starA;
    delete starB;
    delete starC;

    return 0;
}