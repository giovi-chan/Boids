
#include "../include/graphics.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../include/boid.hpp"
#include "../include/flock.hpp"
#include "../include/point.hpp"

namespace graphics {

std::unique_ptr<sf::RenderWindow> makeWindow(unsigned width, unsigned height,
                                             const std::string& title) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  auto window = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(width, height), title, sf::Style::Close, settings);
  window->setVerticalSyncEnabled(true);
  return window;
}

float angleDegFromVelocity(double vx, double vy) {
  float degree = static_cast<float>(std::atan2(vy, vx) * 180.0 / M_PI);
  return degree;
}

sf::ConvexShape makeBoidTriangle(float size, float stroke, sf::Color fill,
                                 sf::Color outline) {
  // Triangolo isoscele centrato sull'origine, punta verso +X
  // Vertici in senso orario: tip, back-top, back-bottom
  const float L = size;         // lunghezza "punta"
  const float W = size * 0.6f;  // larghezza coda

  sf::ConvexShape triangle(3);
  triangle.setPoint(0, sf::Vector2f(+L, 0.f));       // punta
  triangle.setPoint(1, sf::Vector2f(-L * 1.f, +W));  // coda sopra
  triangle.setPoint(2, sf::Vector2f(-L * 1.f, -W));  // coda sotto

  triangle.setFillColor(fill);
  triangle.setOutlineColor(outline);
  triangle.setOutlineThickness(stroke);
  return triangle;
}

void drawBoid(sf::RenderWindow& window, double x, double y, double vx,
              double vy, const Style& style, bool is_prey) {
  sf::ConvexShape triangle =
      is_prey ? makeBoidTriangle(style.prey_size, style.stroke, style.prey_fill,
                                 style.prey_outline)
              : makeBoidTriangle(style.predator_size, style.stroke,
                                 style.predator_fill, style.predator_outline);

  // Posizioniamo al centro (x,y) e ruotiamo verso la velocità
  triangle.setPosition(static_cast<float>(x), static_cast<float>(y));
  float degree = angleDegFromVelocity(vx, vy);
  triangle.setRotation(degree);

  window.draw(triangle);
}

void drawFrame(sf::RenderWindow& window, const flock::Flock& flock,
               const Style& style, const int time) {
  window.clear(/*style.background*/);
  if ((time / 1000) % 2) {
    sf::VertexArray gradient1(sf::Quads, 4);

    gradient1[0].position = sf::Vector2f(0, 0);
    gradient1[1].position = sf::Vector2f(graphics::window_width, 0);
    gradient1[2].position =
        sf::Vector2f(graphics::window_width, graphics::window_height);
    gradient1[3].position = sf::Vector2f(0, graphics::window_height);

    gradient1[0].color = sf::Color(255, 253, 208);  // giallo chiaro
    gradient1[1].color = sf::Color(255, 253, 208);
    gradient1[2].color = sf::Color(135, 206, 250);  // azzurro
    gradient1[3].color = sf::Color(135, 206, 250);

    window.draw(gradient1);
  }

  else {
    sf::VertexArray gradient2(sf::Quads, 4);

    gradient2[0].position = sf::Vector2f(0, 0);
    gradient2[1].position = sf::Vector2f(graphics::window_width, 0);
    gradient2[2].position =
        sf::Vector2f(graphics::window_width, graphics::window_height);
    gradient2[3].position = sf::Vector2f(0, graphics::window_height);

    gradient2[0].color = sf::Color(72, 61, 139);
    gradient2[1].color = sf::Color(72, 61, 139);
    gradient2[2].color = sf::Color(255, 99, 71);
    gradient2[3].color = sf::Color(255, 140, 0);

    window.draw(gradient2);
  }

  for (const auto& prey : flock.getPreyFlock()) {
    const auto pos = prey->getPosition();
    const auto vel = prey->getVelocity();
    drawBoid(window, pos.getX(), pos.getY(), vel.getX(), vel.getY(), style,
             true);
  }

  for (const auto& predator : flock.getPredatorFlock()) {
    const auto pos = predator->getPosition();
    const auto vel = predator->getVelocity();
    drawBoid(window, pos.getX(), pos.getY(), vel.getX(), vel.getY(), style,
             false);
  }

  window.display();
}

}  // namespace graphics
