#ifndef POINT_HPP
#define POINT_HPP

namespace point {
class Point {
  double x_;
  double y_;

 public:
  Point(double x = 0., double y = 0.);

  double x() const;
  double y() const;
  double distance() const;
  double distance(Point const&) const;
};

Point operator+(Point const&, Point const&);
Point operator-(Point const&, Point const&);
Point operator*(Point const&, double);
}

#endif