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

  public:
      Polygon(std::vector<Point> points);
      Polygon(double* points, int size);
      ~Polygon() { delete[] _points; }

      bool isInside(const Point& p);
      bool isSimple();
      bool isConvex();

      static Polygon convexHull(std::vector<Point> points);
};

#endif