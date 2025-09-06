#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "../include/constants.hpp"
#include "../include/flock.hpp"
#include "../include/graphics.hpp"

int main() {
  try {
    const unsigned int W{constants::window_width};
    const unsigned int H{constants::window_height};

    auto window = graphics::makeWindow(W, H, "BOIDS");

    const std::size_t N_PREY = 200;
    const std::size_t N_PRED = 3;
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

      const float dt = simClock.restart().asSeconds();
      F.updateFlock(dt);

      window->clear(sf::Color(20, 20, 30));

      graphics::drawFrame(*window, F, style);
      window->display();
    }
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}
