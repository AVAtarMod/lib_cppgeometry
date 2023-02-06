#ifndef COURSEWORK_4_1_QUADRILATERAL_HPP
#define COURSEWORK_4_1_QUADRILATERAL_HPP

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

#endif // COURSEWORK_4_1_QUADRILATERAL_HPP
