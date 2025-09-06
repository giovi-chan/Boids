
#include "../include/graphics.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../include/boid.hpp"
#include "../include/constants.hpp"  // per window_width/window_height se vuoi usarli qui
#include "../include/flock.hpp"
#include "../include/point.hpp"

using std::unique_ptr;

namespace graphics {

unique_ptr<sf::RenderWindow> makeWindow(unsigned width, unsigned height,
                                        const std::string& title) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  auto win = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(width, height), title, sf::Style::Close, settings);
  win->setVerticalSyncEnabled(true);
  return win;
}

float angleDegFromVelocity(double vx, double vy) {
  // atan2(y,x) -> radianti, convertiamo in gradi. La punta del triangolo è
  // definita lungo +X.
  float deg = static_cast<float>(std::atan2(vy, vx) * 180.0 / M_PI);
  return deg;
}

sf::ConvexShape makeBoidTriangle(float size_px, float outline_px,
                                 sf::Color fill, sf::Color outline) {
  // Triangolo isoscele centrato sull'origine, punta verso +X
  // Vertici in senso orario: tip, back-top, back-bottom
  const float L = size_px;         // lunghezza "punta"
  const float W = size_px * 0.6f;  // larghezza coda

  sf::ConvexShape tri(3);
  tri.setPoint(0, sf::Vector2f(+L, 0.f));        // punta
  tri.setPoint(1, sf::Vector2f(-L * 0.8f, +W));  // coda sopra
  tri.setPoint(2, sf::Vector2f(-L * 0.8f, -W));  // coda sotto

  tri.setFillColor(fill);
  tri.setOutlineColor(outline);
  tri.setOutlineThickness(outline_px);
  return tri;
}

void drawBoid(sf::RenderWindow& window, double x, double y, double vx,
              double vy, const Style& style, bool is_predator) {
  sf::ConvexShape tri =
      makeBoidTriangle(style.size_px, style.stroke_px,
                       is_predator ? style.pred_fill : style.prey_fill,
                       is_predator ? style.pred_outline : style.prey_outline);

  // Posizioniamo al centro (x,y) e ruotiamo verso la velocità
  tri.setPosition(static_cast<float>(x), static_cast<float>(y));
  float deg = angleDegFromVelocity(vx, vy);
  tri.setRotation(deg);

  window.draw(tri);
}

void drawFrame(sf::RenderWindow& window, const flock::Flock& F,
               const Style& style) {
  window.clear(style.bg);

  // Prede
  for (const auto& p : F.getPreyFlock()) {
    const auto pos = p->getPosition();
    const auto vel = p->getVelocity();
    drawBoid(window, pos.getX(), pos.getY(), vel.getX(), vel.getY(), style,
             false);
  }

  // Predatori
  for (const auto& pr : F.getPredatorFlock()) {
    const auto pos = pr->getPosition();
    const auto vel = pr->getVelocity();
    drawBoid(window, pos.getX(), pos.getY(), vel.getX(), vel.getY(), style,
             true);
  }

  window.display();
}

}  // namespace graphics
