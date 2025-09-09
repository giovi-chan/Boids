#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>

#include "../include/flock.hpp"
#include "../include/graphics.hpp"

int main() {
  flock::Flock flock(0, 0);
  flock.setFlockSize();
  flock.setFlightParameters();
  flock.generateBoids();

  auto window = graphics::makeWindow(graphics::window_width,
                                     graphics::window_height, "Boids");

  graphics::Style style;

  sf::Clock simClock;

  if (!graphics::loadBackground("assets/world_map31.png")) {
    std::cerr << "Errore: impossibile caricare lo sfondo. Userò un colore di "
                 "default.\n";
  }

  sf::RectangleShape statsPanel;
  statsPanel.setSize(sf::Vector2f(250.f, 120.f));    // larghezza x altezza
  statsPanel.setFillColor(sf::Color(0, 0, 0, 150));  // nero trasparente
  statsPanel.setPosition(10.f, 10.f);  // posizione in alto a sinistra

  sf::Font font;
  if (!font.loadFromFile("assets/font/Roboto_Condensed-Light.ttf")) {
    std::cerr << "Errore: impossibile caricare il font!\n";
  }

  sf::Text statsText;
  statsText.setFont(font);
  statsText.setCharacterSize(16);
  statsText.setFillColor(sf::Color::White);
  statsText.setPosition(15.f, 15.f);  // leggero offset dal rettangolo

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

    auto stats = flock.statistics();
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Mean dist: " << stats.mean_distance << "\n"
        << "Dev dist: " << stats.dev_distance << "\n"
        << "Mean speed: " << stats.mean_velocity << "\n"
        << "Dev speed: " << stats.dev_velocity;
    statsText.setString(oss.str());

    window->draw(statsPanel);
    window->draw(statsText);

    window->display();
  }
  return 0;
}
