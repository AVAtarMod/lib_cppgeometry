#include "Point.hpp"

#include <cmath>

bool Point::isAtInfinity(const Point& point)
{
   return std::isinf(point._x) || std::isinf(point._y);
}
