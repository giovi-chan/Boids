#include "../include/point.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

#include "../include/constants.hpp"

namespace point {

// methods
Point::Point(double x, double y)
    : x_{x}, y_{y} {}  // definizione del costruttore

double Point::getX() const { return x_; }
double Point::getY() const { return y_; }
void Point::setX(double x) { x_ = x; }
void Point::setY(double y) { y_ = y; }

double Point::distance() const { return std::sqrt(x_ * x_ + y_ * y_); }
double Point::distance(Point const& other) const {
  return std::sqrt((x_ - other.getX()) * (x_ - other.getX()) +
                   (y_ - other.getY()) * (y_ - other.getY()));
}

// free functions
Point operator+(Point const& a, Point const& b) {
  Point sum(a.getX() + b.getX(), a.getY() + b.getY());
  return sum;
}

Point operator-(Point const& a, Point const& b) {
  Point diff(a.getX() - b.getX(), a.getY() - b.getY());
  return diff;
}

Point operator*(Point const& p, double c) {
  Point point(p.getX() * c, p.getY() * c);
  return point;
}

Point operator/(Point const& p, double c) {
  assert(c != 0.);
  Point point(p.getX() / c, p.getY() / c);
  return point;
}

bool operator==(Point const& p, Point const& q) {
  return (p.getX() == q.getX() && p.getY() == q.getY());
}

// free function relative position: dati due punti sulla finestra, calcola il
// vettore p2-p1 che li congiunge, tenendo conto della geometria a toroide

Point relative_position(Point const& p1, Point const& p2) {
  double width = static_cast<double>(constants::window_width);
  double height = static_cast<double>(constants::window_height);
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
    Point const& p) {  // distance from origin of a point in the window
  assert(p.getX() >= 0 && p.getX() <= constants::window_width);
  assert(p.getY() >= 0 && p.getY() <= constants::window_height);
  return relative_position({0., 0.}, p).distance();
}

double toroidal_distance(
    Point const& p, Point const& q) {  // distance of two points in the window
  assert(p.getX() >= 0 && p.getX() <= constants::window_width);
  assert(p.getY() >= 0 && p.getY() <= constants::window_height);
  assert(q.getX() >= 0 && q.getX() <= constants::window_width);
  assert(q.getY() >= 0 && q.getY() <= constants::window_height);
  return relative_position(p, q).distance();
}

}  // namespace point