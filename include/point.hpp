#ifndef POINT_HPP
#define POINT_HPP

namespace point {
class Point {
 private:
  double x_;
  double y_;

  // methods
 public:
  Point(double x = 0., double y = 0.);

  double get_x() const;
  double get_y() const;
  void set_x(double);
  void set_y(double);

  double distance() const;
  double distance(Point const&) const;
};

// free functions
Point operator+(Point const&, Point const&);
Point operator-(Point const&, Point const&);
Point operator*(Point const&, double);
Point operator/(Point const&, double);
bool operator==(Point const&, Point const&);
Point relative_position(const Point&, const Point&);
double toroidal_distance(Point const&);
double toroidal_distance(Point const&, Point const&);

}  // namespace point

#endif