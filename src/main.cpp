#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "core/StarMap.h"
#include "pathfinding/AStarPathfinder.h"
#include "pathfinding/Heuristic.h"
#include "render/Camera.h"
#include "render/Renderer.h"

int main() {
    // Seed the random number generator so we get a different galaxy every time
    srand(time(0)); 

    // Create a 1280x720 window
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "3D Space Map");
    window.setVerticalSyncEnabled(true);

    // Initialize the Renderer and Camera
    Renderer renderer;
    renderer.init();
    Camera camera;

    // ==========================================
    // 1. Generate a random galaxy of 1000 stars
    // ==========================================
    StarMap map;
    for (int i = 0; i < 1000; i++) {
        // Spread the stars out in 3D space
        float x = (rand() % 200 - 100) / 2.0f;
        float y = (rand() % 200 - 100) / 2.0f;
        float z = -(rand() % 200) / 2.0f - 5.0f; // Deep into the screen
        
        map.addStar(new Star("Star", glm::vec3(x, y, z), 1.0f, StarType::MainSequence));
    }

    // Pick the first and last star to find a path between
    Star* startStar = map.getAllStars()[0];
    Star* goalStar = map.getAllStars()[999];
    
    // Connect them (just for testing the path drawing)
    map.addRoute(startStar, goalStar);

    // ==========================================
    // 2. Run A* to find the path
    // ==========================================
    auto heuristic = std::make_unique<EuclideanHeuristic>();
    AStarPathfinder astar(std::move(heuristic));
    std::vector<Star*> path = astar.findPath(&map, startStar, goalStar);

    // ==========================================
    // 3. Main Game Loop
    // ==========================================
    while (window.isOpen()) {
        // Handle window close events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Handle WASD movement (fly around the map)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) camera.moveForward(0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) camera.moveForward(-0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) camera.moveRight(-0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) camera.moveRight(0.1f);

        // Render the stars and the path
        renderer.render(map.getAllStars(), path, camera);

        // Update the window
        window.display();
    }

    return 0;
}