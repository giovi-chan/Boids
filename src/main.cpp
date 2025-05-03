#include <iostream>
#include <random>
#include <vector>

#include "../include/boid.hpp"

double uniform(double a, double b, std::mt19937& mt) {
  std::uniform_real_distribution<double> dist(a, b);
  return dist(mt);
}

void initialize_boids(std::vector<Boid>& boids, int count, double radius_min,
                      double radius_max, double vel_min, double vel_max, 
                      std::mt19937& mt) {
  boids.clear();
  for (int i = 0; i < count; ++i) {
    Point pos(std::cos(uniform(radius_min, radius_max, mt)), std::sin((uniform(radius_min, radius_max, mt))) );
    Point vel(std::cos(uniform(vel_min, vel_max, mt)), std::sin((uniform(vel_min, vel_max, mt))) );
    
    boids.push_back(Boid(pos, vel));
  }
}

int main() {
  std::mt19937 mt(std::random_device{}());

  const int num_boids = 100;
  std::vector<Boid> boid_vector;

  // Intervalli per posizione e velocità
  double radius_min = 0.0, radius_max = 100.0;
  double vel_min = 0.0, vel_max = 1.0;

  initialize_boids(boid_vector, num_boids, radius_min, radius_max, vel_min, vel_max, mt);

  // Stampa tutti i boid
  for (const auto& boid : boid_vector) {
    std::cout << "Posizione: (" << boid.position().x() << ", "
              << boid.position().y() << ")";
    std::cout << " | Velocità: (" << boid.velocity().x() << ", "
              << boid.velocity().y() << ")\n";
  }

  return 0;
}