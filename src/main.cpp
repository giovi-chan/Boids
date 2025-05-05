#include <SFML/Graphics.hpp>
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

std::vector<boid::Boid> get_neighbors(const boid::Boid& current,
                                      const std::vector<boid::Boid>& all_boids,
                                      double radius) {
  std::vector<boid::Boid> neighbors;
  for (const auto& other : all_boids) {
    if (&current != &other &&
        (current.position() - other.position()).distance() < radius) {
      neighbors.push_back(other);
    }
  }
  return neighbors;
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

  sf::RenderWindow window(
      sf::VideoMode(constants::window_width, constants::window_height),
      "Boids Simulation");

  const double delta_t = 5;
  const double neighborhood_radius = 400.0;
  const double separation_dist = 20.0;
  const double separation_coeff = 0.05;
  const double cohesion_coeff = 0.001;
  const double alignment_coeff = 0.05;

  window.setFramerateLimit(60);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    // Update boids
    std::vector<boid::Boid> boid_copy = boid_vector;
    for (auto& b : boid_vector) {
      auto neighbors = get_neighbors(b, boid_copy, neighborhood_radius);
      b.update(delta_t, neighbors, separation_dist, separation_coeff,
               cohesion_coeff, alignment_coeff);
    }

    // Draw boids
    window.clear(constants::window_color);
    for (const auto& b : boid_vector) {
      sf::ConvexShape triangle;
      triangle.setPointCount(3);
      triangle.setPoint(0, sf::Vector2f(0.f, -constants::boid_size));  // punta
      triangle.setPoint(
          1, sf::Vector2f(constants::boid_size / 2, constants::boid_size));
      triangle.setPoint(
          2, sf::Vector2f(-constants::boid_size / 2, constants::boid_size));

      triangle.setFillColor(constants::boid_color);

      // Imposta la posizione
      triangle.setPosition(static_cast<float>(b.position().x()),
                           static_cast<float>(b.position().y()));

      // Calcola l’angolo
      float angle =
      static_cast<float>((std::atan2(b.velocity().y(), b.velocity().x()) * 180 / M_PI) + 90);
      triangle.setRotation(angle);

      window.draw(triangle);
    }
    window.display();
  }

  return 0;
}