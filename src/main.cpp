#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "../include/flock.hpp"
#include "../include/graphics.hpp"

int main() {
  auto window = graphics::makeWindow(graphics::window_width,
                                     graphics::window_height, "Boids");

  const std::size_t N_PREY = 200;
  const std::size_t N_PREDATOR = 5;
  flock::Flock flock(N_PREY, N_PREDATOR);
  flock.generateBoids();

  graphics::Style style;

  sf::Clock simClock;

  int t{0};
  while (window->isOpen()) {
    sf::Event event;
    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed) window->close();
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) window->close();
      }
    }

    const float dt = 20 * simClock.restart().asSeconds();
    ++t;

    flock.updateFlock(dt);

    graphics::drawFrame(*window, flock, style, t);

    window->display();
  }
  return 0;
}
