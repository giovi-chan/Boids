#include "../include/point.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

#include "../include/constants.hpp"            //va incluso qui o nell'hpp?

namespace point {

// methods
Point::Point(double x, double y) : x_{x}, y_{y} {}

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

Point operator/(Point const& p, double c){
  assert (c!=0.);
  Point point(p.get_x() /c, p.get_y() / c);
  return point;
}



double toroidal_distance(Point const& p) {
  assert(p.get_x() >= 0 && p.get_x() <= constants::window_width);
  assert(p.get_y() >= 0 && p.get_y() <= constants::window_height);
  double x_min{std::min(p.get_x(), constants::window_width - p.get_x())};
  double y_min{std::min(p.get_y(), constants::window_height - p.get_y())};
  return std::sqrt(x_min * x_min + y_min * y_min);
}

double toroidal_distance(Point const& p, Point const& q) {
  assert(p.get_x() >= 0 && p.get_x() <= constants::window_width);
  assert(p.get_y() >= 0 && p.get_y() <= constants::window_height);
  assert(q.get_x() >= 0 && q.get_x() <= constants::window_width);
  assert(q.get_y() >= 0 && q.get_y() <= constants::window_height);
  double x_min{
      q.get_x() - p.get_x() >= 0.
          ? std::min(q.get_x() - p.get_x(), constants::window_width - q.get_x() + p.get_x())
          : std::min(p.get_x() - q.get_x(), constants::window_width - p.get_x() + q.get_x())};
  double y_min{
      q.get_y() - p.get_y() >= 0.
          ? std::min(q.get_y() - p.get_y(), constants::window_height - q.get_y() + p.get_y())
          : std::min(p.get_y() - q.get_y(), constants::window_height - p.get_y() + q.get_y())};
  return std::sqrt(x_min * x_min + y_min * y_min);
}

}  // namespace point