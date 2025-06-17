#include "../include/sfml.hpp"

#include <cmath>

namespace graphics {
SimulationWindow::SimulationWindow()
    : window_(sf::VideoMode(constants::window_width, constants::window_height),
              "Boids Simulation"),
      mt_(std::random_device{}()) {
  window_.setFramerateLimit(60);
  initializeBoids();
}

void SimulationWindow::run() {
  const double delta_t = 1.0;

  while (window_.isOpen()) {
    processEvents();
    update(delta_t);
    render();
  }
}

void SimulationWindow::processEvents() {
  sf::Event event;
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed) window_.close();
  }
}

void SimulationWindow::initializeBoids() {
  boids_.clear();
  for (int i = 0; i < constants::num_boids; ++i) {
    double x =
        std::uniform_real_distribution<double>(0, constants::window_width)(mt_);
    double y = std::uniform_real_distribution<double>(
        0, constants::window_height)(mt_);
    point::Point pos(x, y);

    double angle = std::uniform_real_distribution<double>(0, 2 * M_PI)(mt_);
    double speed = std::uniform_real_distribution<double>(
        constants::min_velocity, constants::max_velocity)(mt_);
    point::Point vel(speed * std::cos(angle), speed * std::sin(angle));

    boids_.emplace_back(pos, vel);
  }
}

std::vector<boid::Boid> SimulationWindow::getNeighbors(
    const boid::Boid& current) const {
  std::vector<boid::Boid> neighbors;
  for (const auto& other : boids_) {
    point::Point rel_pos =
        point::relative_position(current.get_position(), other.get_position());
    double angle = current.angle(other);

    if (&current != &other && rel_pos.distance() < neighborhood_radius_ &&
        angle < constants::field_of_view) {
      neighbors.push_back(other);
    }
  }
  return neighbors;
}

void SimulationWindow::update(double delta_t) {
  std::vector<boid::Boid> boids_copy = boids_;
  for (auto& b : boids_) {
    auto neighbors = getNeighbors(b);
    b.update(delta_t, neighbors, separation_dist_, separation_coeff_,
             cohesion_coeff_, alignment_coeff_);
  }
}

void SimulationWindow::render() {
  window_.clear(constants::window_color);

  for (const auto& b : boids_) {
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(0.f, -constants::boid_size));
    triangle.setPoint(
        1, sf::Vector2f(constants::boid_size / 2, constants::boid_size));
    triangle.setPoint(
        2, sf::Vector2f(-constants::boid_size / 2, constants::boid_size));
    triangle.setFillColor(constants::boid_color);

    triangle.setPosition(static_cast<float>(b.get_position().get_x()),
                         static_cast<float>(b.get_position().get_y()));

    float angle = static_cast<float>(
        (std::atan2(b.get_velocity().get_y(), b.get_velocity().get_x()) * 180 /
         M_PI) +
        90);
    triangle.setRotation(angle);

    window_.draw(triangle);
  }

  window_.display();
}

}  // namespace graphics
