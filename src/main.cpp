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
#include "ui/Selection.h" // Include the Selection class!

int main() {
    srand(time(0)); 

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "3D Space Map");
    window.setVerticalSyncEnabled(true);

    Renderer renderer;
    renderer.init();
    Camera camera;
    Selection selection; // Initialize selection

    // 1. Generate 1000 stars
    StarMap map;
    for (int i = 0; i < 1000; i++) {
        float x = (rand() % 200 - 100) / 2.0f;
        float y = (rand() % 200 - 100) / 2.0f;
        float z = -(rand() % 200) / 2.0f - 5.0f;
        map.addStar(new Star("Star", glm::vec3(x, y, z), 1.0f, StarType::MainSequence));
    }

    Star* startStar = map.getAllStars()[0];
    Star* goalStar = map.getAllStars()[999];
    map.addRoute(startStar, goalStar);

    // Pathfinding setup
    auto heuristic = std::make_unique<EuclideanHeuristic>();
    AStarPathfinder astar(std::move(heuristic));
    std::vector<Star*> path = astar.findPath(&map, startStar, goalStar);

    // 2. Load a font for the UI (We will use a default system font for now)
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Could not load font!\n";
    }
    sf::Text uiText(font, "Left Click to Find Path", 20);
    uiText.setFillColor(sf::Color::White);
    uiText.setPosition({10.0f, 10.0f});

    // 3. Main Game Loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            // MOUSE CLICK DETECTION
            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    // Get mouse position
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    
                    // Find the star closest to the click
                    Star* clickedStar = selection.getStarAtScreenPos(mousePos.x, mousePos.y, map.getAllStars(), camera);
                    
                    if (clickedStar) {
                        // Recalculate path from startStar to the clicked star
                        path = astar.findPath(&map, startStar, clickedStar);
                        uiText.setString("Path found to a new star!");
                    }
                }
            }
        }

        // Handle WASD movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) camera.moveForward(0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) camera.moveForward(-0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) camera.moveRight(-0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) camera.moveRight(0.1f);

        // Render 3D
        renderer.render(map.getAllStars(), path, camera);

        // Render 2D UI (Text on top of the 3D scene)
        window.pushGLStates(); // Switch to 2D mode
        window.draw(uiText);
        window.popGLStates();  // Switch back to 3D mode

        window.display();
    }

    return 0;
}