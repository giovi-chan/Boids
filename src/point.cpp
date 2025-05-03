#include "../include/point.hpp"

#include <cmath>

// methods
Point::Point(double x, double y) : x_{x}, y_{y} {}

double Point::x() const { return x_; }
double Point::y() const { return y_; }

double Point::distance() const { return std::sqrt(x_ * x_ + y_ * y_); }

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
