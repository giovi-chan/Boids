#ifndef POINT_HPP
#define POINT_HPP

namespace point {
class Point {
  double x_;
  double y_;

  // methods
 public:
  Point(double x = 0., double y = 0.);   //dichiarazione del costruttore, adesso il compilatore sa che esiste un costruttore pubblico che accetta due double e che presenta 0 come valori di default se uno o entrambi gli argomenti mancano

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
Point relative_position(const Point& , const Point& );
double toroidal_distance(Point const&);
double toroidal_distance(Point const&, Point const&);

}  // namespace point

#endif