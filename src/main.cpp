#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "algorithm"
#include "vector"

#include "core/StarMap.h"
#include "pathfinding/AStarPathfinder.h"
#include "pathfinding/Heuristic.h"
#include "render/Camera.h"
#include "render/Renderer.h"
#include "ui/Selection.h"

int main() {
    srand(time(0)); 

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "3D Space Map");
    window.setVerticalSyncEnabled(true);

    Renderer renderer;
    renderer.init();
    Camera camera;
    Selection selection;

    // 1. Generate 1000 stars
    StarMap map;
    for (int i = 0; i < 1000; i++) {
        float x = (rand() % 200 - 100) / 2.0f;
        float y = (rand() % 200 - 100) / 2.0f;
        float z = -(rand() % 200) / 2.0f - 5.0f;
        map.addStar(new Star("Star", glm::vec3(x, y, z), 1.0f, StarType::MainSequence));
    }

    // 2. Connect each star to its 3 closest neighbors (creates a realistic space map)
    auto allStars = map.getAllStars();
    for (size_t i = 0; i < allStars.size(); i++) {
        Star* current = allStars[i];
        
        // Find neighbors and sort by distance
        std::vector<std::pair<float, Star*>> distances;
        for (size_t j = 0; j < allStars.size(); j++) {
            if (i == j) continue;
            distances.push_back({map.getDistance(current, allStars[j]), allStars[j]});
        }
        std::sort(distances.begin(), distances.end());

        // Connect to the 3 closest stars
        for (int k = 0; k < 3 && k < distances.size(); k++) {
            map.addRoute(current, distances[k].second);
        }
    }

    Star* startStar = map.getAllStars()[0];
    Star* goalStar = map.getAllStars()[999];
    
    // Pathfinding setup
    auto heuristic = std::make_unique<EuclideanHeuristic>();
    AStarPathfinder astar(std::move(heuristic));
    std::vector<Star*> path = astar.findPath(&map, startStar, goalStar);

    // 3. Load a font for the UI
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Could not load font!\n";
    }
    sf::Text uiText(font, "Left Click to Find Path", 20);
    uiText.setFillColor(sf::Color::White);
    uiText.setPosition({10.0f, 10.0f});

    // 4. Main Game Loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    
                    Star* clickedStar = selection.getStarAtScreenPos(mousePos.x, mousePos.y, map.getAllStars(), camera);
                    
                    if (clickedStar) {
                        path = astar.findPath(&map, startStar, clickedStar);
                        
                        float totalDistance = 0.0f;
                        if (path.size() > 1) {
                            for (int i = 0; i < path.size() - 1; ++i) {
                                totalDistance += map.getDistance(path[i], path[i+1]);
                            }
                        }
                        
                        std::string distString = std::to_string(totalDistance);
                        distString = distString.substr(0, distString.find('.') + 3); 
                        uiText.setString("Path found! Distance: " + distString + " units");
                    }
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) camera.moveForward(0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) camera.moveForward(-0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) camera.moveRight(-0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) camera.moveRight(0.1f);

        renderer.render(map.getAllStars(), path, camera);

        window.pushGLStates(); 
        window.draw(uiText);
        window.popGLStates();  

        window.display();
    }

    return 0;
}