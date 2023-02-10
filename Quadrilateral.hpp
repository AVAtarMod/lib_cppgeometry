#ifndef GEOMETRY_LIB_QUADRILATERAL_HPP
#define GEOMETRY_LIB_QUADRILATERAL_HPP

#include "Point.hpp"

class Quadrilateral
{
  private:
   Point A, B, C, D;

  public:
   Quadrilateral(const Point& a, const Point& b, const Point& c,
                 const Point& d);
   ~Quadrilateral();
};

#endif // GEOMETRY_LIB_QUADRILATERAL_HPP
