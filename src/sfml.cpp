#include "../include/sfml.hpp"

#include <SFML/Graphics.hpp>

void render_boids(const std::vector<boid::Boid>& boids, int window_width,
                  int window_height) {
  sf::RenderWindow window(sf::VideoMode(window_width, window_height),
                          "Boids Simulation");

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    window.clear(constants::window_color);

    for (const auto& boid : boids) {
      sf::CircleShape shape(constants::boid_size);  // cerchio
      shape.setFillColor(constants::boid_color);
      shape.setPosition(static_cast<float>(boid.position().x()),
                        static_cast<float>(boid.position().y()));
      window.draw(shape);
    }

    window.display();
  }
}
