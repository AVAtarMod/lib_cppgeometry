#include "Quadrilateral.hpp"

#include "Line.hpp"

Quadrilateral::Quadrilateral(const Point& a, const Point& b, const Point& c,
                             const Point& d)
{
   if (Line::isOnSameLine(a, b, c) || Line::isOnSameLine(a, b, d) ||
       Line::isOnSameLine(a, c, d) || Line::isOnSameLine(b, c, d)) {
      throw std::invalid_argument(
        "Quadrilateral: cannot construct object. Points must be not on same line.");
   }
   A = a, B = b, C = c, D = d;
}

Quadrilateral::~Quadrilateral()
{
   A.~Point();
   B.~Point();
   C.~Point();
   D.~Point();
}
