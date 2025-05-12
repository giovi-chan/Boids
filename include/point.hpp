#ifndef POINT_HPP
#define POINT_HPP

namespace point {
class Point {
  double x_;
  double y_;

  // methods
 public:
  Point(double x = 0., double y = 0.);

  double get_x() const;         // getters
  double get_y() const;
  void set_x(double);       //setters
  void set_y(double);

  double distance() const;
  double distance(Point const&) const;
};

// free functions
Point operator+(Point const&, Point const&);
Point operator-(Point const&, Point const&);
Point operator*(Point const&, double);
Point operator/(Point const&, double);
double toroidal_distance(Point const&);
double toroidal_distance(Point const&, Point const&);

}  // namespace point

#endif