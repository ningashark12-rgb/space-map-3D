#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
        
        // Give some stars different names to make it look cool
        std::string starName = "Star";
        if (i % 10 == 0) starName = "Sun";
        else if (i % 7 == 0) starName = "Nova";
        else if (i % 5 == 0) starName = "Vega";
        
        map.addStar(new Star(starName + " " + std::to_string(i), glm::vec3(x, y, z), 1.0f, StarType::MainSequence));
    }

    // 2. Connect each star to its 3 closest neighbors
    auto allStars = map.getAllStars();
    for (size_t i = 0; i < allStars.size(); i++) {
        Star* current = allStars[i];
        
        std::vector<std::pair<float, Star*>> distances;
        for (size_t j = 0; j < allStars.size(); j++) {
            if (i == j) continue;
            distances.push_back({map.getDistance(current, allStars[j]), allStars[j]});
        }
        std::sort(distances.begin(), distances.end());

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

    // Star label text setup
    sf::Text starText(font, "", 12);
    starText.setFillColor(sf::Color(150, 200, 255)); // Light blue

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

        // Render 3D
        renderer.render(map.getAllStars(), path, camera);

        // Render 2D UI and Star Labels
        window.pushGLStates(); 
        window.draw(uiText);

        // Project 3D stars to 2D screen for labels
        glm::mat4 view = camera.getViewMatrix();
        float aspect = 1280.0f / 720.0f;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        // Only loop through the first 100 stars so the text doesn't lag the game
        for (int i = 0; i < 100 && i < map.getAllStars().size(); i++) {
            Star* star = map.getAllStars()[i];
            glm::vec3 pos = star->getPosition();

            glm::vec4 clipSpace = projection * view * glm::vec4(pos, 1.0f);
            if (clipSpace.w > 0.0f) {
                glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;
                float screenX = (ndc.x + 1.0f) * 0.5f * 1280.0f;
                float screenY = (1.0f - ndc.y) * 0.5f * 720.0f;
                starText.setPosition({screenX + 10.0f, screenY - 10.0f});
                
                if (star->getName().find("Star") == std::string::npos) {
                    // Only draw labels for the special stars (Sun, Nova, Vega)
                    // If you want all stars to have labels, just remove this 'if'
                    starText.setString(star->getName());
                    window.draw(starText);
                }
            }
        }

        window.popGLStates();  

        window.display();
    }

    return 0;
}