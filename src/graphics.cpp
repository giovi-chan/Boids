
#include "../include/graphics.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../include/boid.hpp"
#include "../include/flock.hpp"
#include "../include/point.hpp"

namespace graphics {

sf::Texture backgroundTexture;
sf::Sprite backgroundSprite;

bool loadBackground(const std::string& filename) {
  if (!backgroundTexture.loadFromFile(filename)) {
    return false;  // errore se il file non si carica
  }
  backgroundSprite.setTexture(backgroundTexture);
  const float scaleX = static_cast<float>(window_width) /
                       static_cast<float>(backgroundTexture.getSize().x);
  const float scaleY = static_cast<float>(window_height) /
                       static_cast<float>(backgroundTexture.getSize().y);
  backgroundSprite.setScale(scaleX, scaleY);
  return true;
}

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
               const Style& style) {
  if (backgroundTexture.getSize().x > 0 && backgroundTexture.getSize().y > 0) {
    window.draw(backgroundSprite);
  } else {
    window.clear(style.background);
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
}

}  // namespace graphics
