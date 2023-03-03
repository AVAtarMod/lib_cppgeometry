#include "Polygon.hpp"

Polygon::Polygon(const std::vector<Point>& points)
{
   _size = points.size();
   _points = new Point[_size] {};
   for (uint i = 0; i < _size; ++i) {
      _points[i] = points[i];
   }
}

Polygon::Polygon(Point* points, int size)
{
   _points = new Point[size] {};
   _size = size;
   std::copy_n(points, _size, _points);
}

Point Polygon::operator[](int ind) const
{
   ind %= _size;
   if (ind < 0)
      ind += _size;
   return _points[ind];
}

Point& Polygon::operator[](int ind)
{
   ind %= _size;
   if (ind < 0)
      ind += _size;
   return _points[ind];
}

bool Polygon::isInside(const Point& p) const
{
   return true;
}

bool Polygon::isSimple() const
{
   return true;
}

bool Polygon::isConvex() const
{
   return true;
}

Polygon Polygon::convexHull(const std::vector<Point>& points)
{
   return Polygon(points);
}
