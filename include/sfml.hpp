#ifndef SFML_HPP
#define SFML_HPP

#include <vector>

#include "boid.hpp"
#include "constants.hpp"

void render_boids(const std::vector<boid::Boid>& boids, int window_width,
                  int window_height);

#endif
