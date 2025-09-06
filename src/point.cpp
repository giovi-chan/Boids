#include "../include/point.hpp"

#include <cassert>
#include <cmath>

#include "../include/constants.hpp"

namespace point {

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

Point relativePosition(const Point& p1, const Point& p2) {
  double width = static_cast<double>(constants::window_width);
  double height = static_cast<double>(constants::window_height);
  double half_width = width / 2.0;
  double half_height = height / 2.0;

  double delta_x = p2.getX() - p1.getX();
  double delta_y = p2.getY() - p1.getY();

  if (delta_x > half_width)
    delta_x -= width;
  else if (delta_x < -half_width)
    delta_x += width;

  if (delta_y > half_height)
    delta_y -= height;
  else if (delta_y < -half_height)
    delta_y += height;

  return Point{delta_x, delta_y};
}

double toroidalDistance(const Point& p, const Point& q) {
  return relativePosition(p, q).distance();
}

}  // namespace point
