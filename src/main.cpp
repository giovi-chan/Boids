#include <iostream>
#include <random>
#include <vector>

#include "../include/boid.hpp"
#include "../include/constants.hpp"
#include "../include/sfml.hpp"

double uniform(double a, double b, std::mt19937& mt) {
  std::uniform_real_distribution<double> dist(a, b);
  return dist(mt);
}

void initialize_boids(std::vector<boid::Boid>& boids, int count, double x_min,
                      double x_max, double y_min, double y_max, double vel_min,
                      double vel_max, std::mt19937& mt) {
  boids.clear();
  for (int i = 0; i < count; ++i) {
    point::Point pos(uniform(x_min, x_max, mt), uniform(y_min, y_max, mt));
    double vel_angle = uniform(0, 2 * M_PI, mt);
    double vel_magnitude = uniform(vel_min, vel_max, mt);
    point::Point vel(vel_magnitude * std::cos(vel_angle),
              vel_magnitude * std::sin(vel_angle));

    boids.push_back(boid::Boid(pos, vel));
  }
}

int main() {
  std::mt19937 mt(std::random_device{}());

  const int num_boids = 100;
  std::vector<boid::Boid> boid_vector;

  double x_min = 0.0, x_max = constants::window_width;
  double y_min = 0.0, y_max = constants::window_height;
  double vel_min = constants::min_velocity, vel_max = constants::max_velocity;

  initialize_boids(boid_vector, num_boids, x_min, x_max, y_min, y_max, vel_min,
                   vel_max, mt);

  render_boids(boid_vector, constants::window_width, constants::window_height);

  for (const auto& boid : boid_vector) {
    std::cout << "Posizione: (" << boid.position().x() << ", "
              << boid.position().y() << ")";
    std::cout << " | Velocità: (" << boid.velocity().x() << ", "
              << boid.velocity().y() << ")\n";
  }

  return 0;
}