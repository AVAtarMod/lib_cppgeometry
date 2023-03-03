#ifndef GEOMETRY_LIB_POLYGON_HPP
#define GEOMETRY_LIB_POLYGON_HPP

#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <iostream>
#include <vector>

#include "Point.hpp"

class Polygon
{
  private:
      Point* _points = nullptr;
      int _size;
      int isIntersection(const Point& p, int s1, int s2, int ind) const;

  public:
      Polygon(const std::vector<Point>& points);
      Polygon(Point* points, int size);
      ~Polygon() { delete[] _points; }

      Point operator[](int ind) const;
      Point& operator[](int ind);

      const int size() const { return _size; }

      bool isInside(const Point& p) const;
      bool isSimple() const;
      bool isConvex() const;

      static Polygon convexHull(const std::vector<Point>& points);
};

#endif
