#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics/Color.hpp>

namespace constants {

// Dimensioni finestra
inline constexpr unsigned int window_width = 1200;
inline constexpr unsigned int window_height = 800;

// Dimensione triangoli dei boid
inline constexpr float boid_size = 6.0f;

// Colore  (RGB)
inline const sf::Color boid_color = sf::Color::Cyan;
inline const sf::Color window_color = sf::Color::Black;

// Velocità iniziali
inline constexpr double min_velocity = 0.;
inline constexpr double max_velocity = 1.0;

}  // namespace constants

#endif
