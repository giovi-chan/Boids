#include "../include/point.hpp"

#include <cmath>

namespace point {

// methods
Point::Point(double x, double y) : x_{x}, y_{y} {}

double Point::x() const { return x_; }
double Point::y() const { return y_; }
void Point::set_x(double x) { x_ = x; }
void Point::set_y(double y) { y_ = y; }

double Point::distance() const { return std::sqrt(x_ * x_ + y_ * y_); }
double Point::distance(Point const& other) const {
  return std::sqrt((x_ - other.x()) * (x_ - other.x()) +
                   (y_ - other.y()) * (y_ - other.y()));
}

// free functions
Point operator+(Point const& a, Point const& b) {
  Point sum(a.x() + b.x(), a.y() + b.y());
  return sum;
}

Point operator-(Point const& a, Point const& b) {
  Point diff(a.x() - b.x(), a.y() - b.y());
  return diff;
}

Point operator*(Point const& p, double c) {
  Point point(p.x() * c, p.y() * c);
  return point;
}
}  // namespace point