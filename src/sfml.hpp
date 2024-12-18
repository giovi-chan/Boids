
// #ifndef SFML_HPP
// #define SFML_HPP
// #include <SFML/Graphics.hpp>
// #include <string>
// #include <vector>

// #include "boid.hpp"
// #include "point.hpp"
// namespace boids {
// // template function, so it can handle both boids and predators
// // updates the position and orientation of the triangle representing the
// // provided boid
// // Param 1: vertex array of boids/predators
// // Param 2: the boid/predator
// // Param 3: the position of the first vertex of the boid/predator in the vertex
// // array
// // Param 4: the size of the boid/predator

// template <class T>
// void vertex_update(sf::VertexArray& swarm_vertex, const T& bird, int index,
//                    double size) {
//   Point forward_vertex{0., 0.};

//   // to prevent division by zero.
//   if ((bird.vel()).distance() != 0.)
//     forward_vertex = (size / bird.vel().distance()) * (bird.vel());

//   swarm_vertex[3 * index].position =
//       // i have added the *2 so the front is longer,
//       // and we can distinguish it.
//       sf::Vector2f((bird.pos() + 2 * forward_vertex).x(),
//                    (bird.pos() + 2 * forward_vertex).y());

//   // rotate by 120
//   forward_vertex.rotate(2. / 3 * constants::pi);

//   swarm_vertex[(3 * index) + 1].position = sf::Vector2f(
//       (bird.pos() + forward_vertex).x(), (bird.pos() + forward_vertex).y());

//   forward_vertex.rotate(2. / 3 * constants::pi);

//   swarm_vertex[(3 * index) + 2].position = sf::Vector2f(
//       (bird.pos() + forward_vertex).x(), (bird.pos() + forward_vertex).y());
// }

// // displays a circle of provided radius, centered on the position of a provided
// // boid.
// // Param 1: the window object on which to draw the circle
// // Param 2: the radious of the circle
// // Param 3: the boid
// // Param 4: the color of the circle
// void display_circle(sf::RenderWindow&, double, Boid&, sf::Color color);
// }  // namespace boids
// #endif