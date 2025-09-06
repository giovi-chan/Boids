#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// forward declarations per evitare include pesanti
namespace boid {
class Boid;
}
namespace flock {
class Flock;
}

namespace graphics {

// Parametri di stile per i boids
struct Style {
  float size_px = 10.f;    // "raggio" dei triangoli
  float stroke_px = 1.2f;  // spessore contorno
  sf::Color prey_fill = sf::Color(80, 160, 255);
  sf::Color prey_outline = sf::Color(30, 80, 160);
  sf::Color pred_fill = sf::Color(255, 100, 100);
  sf::Color pred_outline = sf::Color(160, 40, 40);
  sf::Color bg = sf::Color(18, 18, 22);
};

std::unique_ptr<sf::RenderWindow> makeWindow(unsigned int width,
                                             unsigned int height,
                                             const std::string& title);

void drawFrame(sf::RenderWindow& window, const flock::Flock& F,
               const Style& style);

// Converte (vx,vy) -> gradi per ruotare il triangolo
float angleDegFromVelocity(double vx, double vy);

// Costruisce un triangolo isoscele centrato in (0,0) e con punta verso +X; poi
// useremo trasformazioni
sf::ConvexShape makeBoidTriangle(float size_px, float outline_px,
                                 sf::Color fill, sf::Color outline);

// Disegna un singolo boid (triangolo traslato + ruotato)
void drawBoid(sf::RenderWindow& window, double x, double y, double vx,
              double vy, const Style& style, bool is_prey);

}  // namespace graphics

#endif