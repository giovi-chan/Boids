#include "../include/point.hpp"

#include <cassert>
#include <cmath>  // for std::sqrt

#include "../include/sfml.hpp"  //for graphic parameter

namespace point {

// methods
Point::Point(double x, double y) : x_{x}, y_{y} {}

double Point::getX() const { return x_; }
double Point::getY() const { return y_; }
void Point::setX(double x) { x_ = x; }
void Point::setY(double y) { y_ = y; }

double Point::distance() const { return std::sqrt(x_ * x_ + y_ * y_); }
double Point::distance(Point const& other) const {
  return std::sqrt((x_ - other.getX()) * (x_ - other.getX()) +
                   (y_ - other.getY()) * (y_ - other.getY()));
}

Point& Point::operator+=(const Point& a) {
  x_ += a.getX();
  y_ += a.getY();
  return *this;
}

// free functions
Point operator+(Point const& a, Point const& b) {
  return Point(a.getX() + b.getX(), a.getY() + b.getY());
}

Point operator-(Point const& a, Point const& b) {
  return Point(a.getX() - b.getX(), a.getY() - b.getY());
}

Point operator*(Point const& p, double c) {
  return Point(p.getX() * c, p.getY() * c);
}

Point operator/(Point const& p, double c) {
  assert(c != 0.);
  return Point(p.getX() / c, p.getY() / c);
}

bool operator==(Point const& p, Point const& q) {
  return (p.getX() == q.getX() && p.getY() == q.getY());
}

// relative position: dati due punti sulla finestra, calcola il
// vettore p2-p1 che li congiunge, tenendo conto della geometria a toroide

Point relative_position(Point const& p1, Point const& p2) {
  double width = static_cast<double>(graphic::window_width);
  double height = static_cast<double>(graphic::window_height);
  double half_width = width / 2.0;
  double half_height = height / 2.0;

  double delta_x = p2.getX() - p1.getX();
  double delta_y = p2.getY() - p1.getY();

  if (delta_x > half_width) {
    delta_x -= width;
  } else if (delta_x < -half_width) {
    delta_x += width;
  }

  if (delta_y > half_height) {
    delta_y -= height;
  } else if (delta_y < -half_height) {
    delta_y += height;
  }

  return Point{delta_x, delta_y};
}

double toroidal_distance(
    Point const& p) {  // distance of a point in the window from origin
  assert(p.getX() >= 0 && p.getX() <= graphic::window_width);
  assert(p.getY() >= 0 && p.getY() <= graphic::window_height);
  return relative_position({0., 0.}, p).distance();
}

double toroidal_distance(
    Point const& p,
    Point const& q) {  // distance between two points in the window
  assert(p.getX() >= 0 && p.getX() <= graphic::window_width);
  assert(p.getY() >= 0 && p.getY() <= graphic::window_height);
  assert(q.getX() >= 0 && q.getX() <= graphic::window_width);
  assert(q.getY() >= 0 && q.getY() <= graphic::window_height);
  return relative_position(p, q).distance();
}

}  // namespace point