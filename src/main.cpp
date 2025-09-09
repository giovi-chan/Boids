#include <SFML/Graphics.hpp>
// #include <iostream>
// #include <string>

#include "../include/flock.hpp"
#include "../include/graphics.hpp"

int main() {
  flock::Flock flock(0, 0);  // creo un Flock vuoto
  flock.setFlockSize(std::cin, std::cout);
  flock.setFlightParameters(std::cin, std::cout);
  flock.generateBoids();

  auto window = graphics::makeWindow(graphics::window_width,
                                     graphics::window_height, "Boids");

  graphics::Style style;

  sf::Clock simClock;

  if (!graphics::loadBackground("assets/world_map31.png")) {
    std::cerr << "Errore: impossibile caricare lo sfondo. Userò un colore di "
                 "default.\n";
  }

  while (window->isOpen()) {
    sf::Event event;
    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed) window->close();
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) window->close();
      }
    }

    const float dt = 20 * simClock.restart().asSeconds();

    flock.updateFlock(dt);

    graphics::drawFrame(*window, flock, style);

    window->display();
  }
  return 0;
}
