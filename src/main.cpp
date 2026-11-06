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

    // 3. Create a dummy map for testing
    StarMap map;
    Star* starA = new Star("A", glm::vec3(0, 0, -5), 1.0f, StarType::MainSequence);
    Star* starB = new Star("B", glm::vec3(2, 1, -10), 1.0f, StarType::MainSequence);
    Star* starC = new Star("C", glm::vec3(-2, -1, -15), 1.0f, StarType::MainSequence);
    
    map.addStar(starA);
    map.addStar(starB);
    map.addStar(starC);
    map.addRoute(starA, starB);
    map.addRoute(starB, starC);

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