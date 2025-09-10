#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "../include/flock.hpp"

namespace graphics {

inline constexpr unsigned int window_width = 1400;
inline constexpr unsigned int window_height = 800;

struct Style {
  float prey_size = 3.f;
  float predator_size = 4.f;
  float stroke = 2.f;
  sf::Color prey_fill = sf::Color(0, 160, 255);
  sf::Color prey_outline = sf::Color(30, 80, 160);
  sf::Color predator_fill = sf::Color(255, 100, 100);
  sf::Color predator_outline = sf::Color(160, 40, 40);
  sf::Color background = sf::Color(255, 255, 204);
};

bool loadBackground(const std::string& filename);
void clearBackground();

std::unique_ptr<sf::RenderWindow> makeWindow(unsigned int width,
                                             unsigned int height,
                                             const std::string& title);

float angleDegFromVelocity(double vx, double vy);

sf::ConvexShape makeBoidTriangle(float size, float stroke, sf::Color fill,
                                 sf::Color outline);

void drawBoid(sf::RenderWindow& window, double x, double y, double vx,
              double vy, const Style& style, bool is_prey);

void drawFrame(sf::RenderWindow& window, const flock::Flock& flock,
               const Style& style);

}  // namespace graphics

#endif