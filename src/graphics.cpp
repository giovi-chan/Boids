#include "../include/graphics.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>

#include "../include/boid.hpp"
#include "../include/flock.hpp"
#include "../include/point.hpp"

namespace graphics {

sf::Texture backgroundTexture;
sf::Sprite backgroundSprite;

bool loadBackground(const std::string& filename) {
  if (!backgroundTexture.loadFromFile(filename)) {
    return false;
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
  settings.antialiasingLevel = 4;
  auto window = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(width, height), title, sf::Style::Close, settings);
  return window;
}

float angleDegFromVelocity(double vx, double vy) {
  return static_cast<float>(std::atan2(vy, vx) * 180.0 / M_PI);
}

sf::ConvexShape makeBoidTriangle(float size, float stroke, sf::Color fill,
                                 sf::Color outline) {
  const float L = size;
  const float W = size * 0.6f;

  sf::ConvexShape triangle(3);
  triangle.setPoint(0, sf::Vector2f(+L, 0.f));
  triangle.setPoint(1, sf::Vector2f(-L, +W));
  triangle.setPoint(2, sf::Vector2f(-L, -W));

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

  triangle.setPosition(static_cast<float>(x), static_cast<float>(y));
  triangle.setRotation(angleDegFromVelocity(vx, vy));

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
