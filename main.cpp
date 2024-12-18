#include <SFML/Graphics.hpp>
#include "boid.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>


int main() {
    // Impostazioni di base
    const int window_width = 800;
    const int window_height = 600;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Boid Simulation");

    // Inizializza i boid
    std::vector<Boid> boids;
    const int num_boids = 100;
    std::srand(std::time(nullptr));
    for (int i = 0; i < num_boids; ++i) {
        Point position(std::rand() % window_width, std::rand() % window_height);
        Point velocity((std::rand() % 200 - 100) / 100.0, (std::rand() % 200 - 100) / 100.0);
        boids.emplace_back(position, velocity);
    }

    // Loop principale
    sf::Clock clock;
    while (window.isOpen()) {
        // Gestione eventi
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Aggiorna boid
        double delta_t = clock.restart().asSeconds();
        for (auto& boid : boids) {
            boid.update(delta_t, boids, 50.0, 1.5, 1.0, 1.0);
        }

        // Disegna i boid
        window.clear();
       for (const auto& boid : boids) {
    // Disegna ogni boid come un triangolo
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(0, 0));       // Vertice
    triangle.setPoint(1, sf::Vector2f(-5, 10));    // Base sinistra
    triangle.setPoint(2, sf::Vector2f(5, 10));     // Base destra
    triangle.setFillColor(sf::Color::Blue);

    // Posizione e rotazione
    Point pos = boid.position();
    Point vel = boid.velocity();
    float angle = std::atan2(vel.y(), vel.x()) * 180 / 3.14159; // Angolo in gradi

    triangle.setPosition(pos.x(), pos.y());
    triangle.setRotation(angle);  // Ruota il triangolo nella direzione del movimento
    window.draw(triangle);
}

        window.display();
    }

    return 0;
}
