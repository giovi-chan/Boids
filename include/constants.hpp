#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics/Color.hpp>

namespace constants {
// pi
constexpr double PI = 3.14159265358979323846;

// Dimensioni finestra
inline constexpr unsigned int window_width = 1200;
inline constexpr unsigned int window_height = 800;

// numero di boids
inline constexpr int num_boids = 100;

// angolo di vista
inline constexpr double field_of_view =  PI / 3;

// Dimensione triangoli dei boid
inline constexpr float boid_size = 6.0f;

// Colore  (RGB)
inline const sf::Color boid_color = sf::Color::White;
inline const sf::Color window_color = sf::Color::Black;

// Velocità iniziali
inline constexpr double min_velocity = 0.;
inline constexpr double max_velocity = 5.0;

}  // namespace constants

#endif
