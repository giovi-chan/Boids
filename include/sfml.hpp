#ifndef SFML_HPP
#define SFML_HPP

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

#include "../include/boid.hpp"
#include "../include/constants.hpp"

namespace graphics {
class SimulationWindow {
 public:
  SimulationWindow();
  void run();

 private:
  void processEvents();
  void update(double delta_t);
  void render();

  std::vector<boid::Boid> boids_;
  sf::RenderWindow window_;

  std::mt19937 mt_;

  // Parametri, saranno la parte privata della gui quando e se ne creerò una
  const double neighborhood_radius_ = 50.0;
  const double separation_dist_ = 10.0;
  const double separation_coeff_ = 0.;
  const double cohesion_coeff_ = 0.1;
  const double alignment_coeff_ = 0.;

  void initializeBoids();  // diventeranno un metodo di flock quando ne creerò i
                           // files e la classe apposita
  std::vector<boid::Boid> getNeighbors(const boid::Boid& current) const;
};
}  // namespace graphics
#endif