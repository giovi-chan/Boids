#ifndef POINT_HPP
#define POINT_HPP

namespace point {
class Point {
 private:
  double x_;
  double y_;

  // methods
 public:
  explicit Point(double x = 0., double y = 0.);

  double getX() const;
  double getY() const;
  void setX(double);
  void setY(double);

  double distance() const;
  double distance(Point const&) const;
  Point& operator+=(Point const&);
};

// free functions
Point operator+(Point const&, Point const&);
Point operator-(Point const&, Point const&);
Point operator*(double, Point const&);
Point operator/(Point const&, double);
bool operator==(Point const&, Point const&);
Point relativePosition(const Point&, const Point&);
double toroidalDistance(Point const&, Point const&);

}  // namespace point

#endif