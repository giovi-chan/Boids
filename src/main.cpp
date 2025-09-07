#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "../include/flock.hpp"
#include "../include/graphics.hpp"

int main() {
  try {
    auto window = graphics::makeWindow(graphics::window_width,
                                       graphics::window_height, "Boids");

    const std::size_t N_PREY = 200;
    const std::size_t N_PRED = 0;
    flock::Flock F(N_PREY, N_PRED);
    F.generateBoids();

    graphics::Style style;

    sf::Clock simClock;

    while (window->isOpen()) {
      sf::Event event;
      while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) window->close();
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::Escape) window->close();
          if (event.key.code == sf::Keyboard::R) F.generateBoids();
        }
      }

      const float dt = 25 * simClock.restart().asSeconds();

      F.updateFlock(dt);

      graphics::drawFrame(*window, F, style);

      window->display();
    }
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}
