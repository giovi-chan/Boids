#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "../include/flock.hpp"

namespace graphics {

inline constexpr unsigned int window_width = 1500;
inline constexpr unsigned int window_height = 850;

struct Style {
  float prey_size = 2.f;
  float predator_size = 2.4f;
  float stroke = 1.4f;
  sf::Color prey_fill = sf::Color(80, 160, 255);
  sf::Color prey_outline = sf::Color(30, 80, 160);
  sf::Color predator_fill = sf::Color(255, 100, 100);
  sf::Color predator_outline = sf::Color(160, 40, 40);
  sf::Color background = sf::Color(255, 255, 204);
};

bool loadBackground(const std::string& filename);

std::unique_ptr<sf::RenderWindow> makeWindow(unsigned int width,
                                             unsigned int height,
                                             const std::string& title);

void drawFrame(sf::RenderWindow& window, const flock::Flock& flock,
               const Style& style);

// Converte (vx,vy) -> gradi per ruotare il triangolo
float angleDegFromVelocity(double vx, double vy);

// Costruisce un triangolo isoscele centrato in (0,0) e con punta verso +X; poi
// useremo trasformazioni
sf::ConvexShape makeBoidTriangle(float size, float stroke, sf::Color fill,
                                 sf::Color outline);

// Disegna un singolo boid (triangolo traslato + ruotato)
void drawBoid(sf::RenderWindow& window, double x, double y, double vx,
              double vy, const Style& style, bool is_prey);

}  // namespace graphics

#endif