#include "../include/point.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

#include "../include/constants.hpp"

namespace point {

// methods
Point::Point(double x, double y)
    : x_{x}, y_{y} {}  // definizione del costruttore

double Point::get_x() const { return x_; }
double Point::get_y() const { return y_; }
void Point::set_x(double x) { x_ = x; }
void Point::set_y(double y) { y_ = y; }

double Point::distance() const { return std::sqrt(x_ * x_ + y_ * y_); }
double Point::distance(Point const& other) const {
  return std::sqrt((x_ - other.get_x()) * (x_ - other.get_x()) +
                   (y_ - other.get_y()) * (y_ - other.get_y()));
}

// free functions
Point operator+(Point const& a, Point const& b) {
  Point sum(a.get_x() + b.get_x(), a.get_y() + b.get_y());
  return sum;
}

Point operator-(Point const& a, Point const& b) {
  Point diff(a.get_x() - b.get_x(), a.get_y() - b.get_y());
  return diff;
}

Point operator*(Point const& p, double c) {
  Point point(p.get_x() * c, p.get_y() * c);
  return point;
}

Point operator/(Point const& p, double c) {
  assert(c != 0.);
  Point point(p.get_x() / c, p.get_y() / c);
  return point;
}

bool operator==(Point const& p, Point const& q) {
  return (p.get_x() == q.get_x() && p.get_y() == q.get_y());
}

// free function relative position: dati due punti sulla finestra, calcola il
// vettore p2-p1 che li congiunge, tenendo conto della geometria a toroide

Point relative_position(Point const& p1, Point const& p2) {
  double width = static_cast<double>(constants::window_width);
  double height = static_cast<double>(constants::window_height);
  double half_width = width / 2.0;
  double half_height = height / 2.0;

  double delta_x = p2.get_x() - p1.get_x();
  double delta_y = p2.get_y() - p1.get_y();

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
  assert(p.get_x() >= 0 && p.get_x() <= constants::window_width);
  assert(p.get_y() >= 0 && p.get_y() <= constants::window_height);
  return relative_position({0., 0.}, p).distance();
}

double toroidal_distance(
    Point const& p, Point const& q) {  // distance of two points in the window
  assert(p.get_x() >= 0 && p.get_x() <= constants::window_width);
  assert(p.get_y() >= 0 && p.get_y() <= constants::window_height);
  assert(q.get_x() >= 0 && q.get_x() <= constants::window_width);
  assert(q.get_y() >= 0 && q.get_y() <= constants::window_height);
  return relative_position(p, q).distance();
}

}  // namespace point