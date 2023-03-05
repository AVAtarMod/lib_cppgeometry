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

      /**
       * @brief Calculates the count of intersections of the ray and the polygon
       * 
       * @param p The point from which the ray originates
       * @param signs Values vector -1, 0, 1. Determine the y-coordinate of a polygon point relative to the y-coordinate of a point p
       * @return Count of intersections
       *
       */
      int static countIntersections(const Point& p, const std::vector<int>& signs);
      /**
       * @brief Checks if the intersection point is located on the right relative to p?
       *
       */
      int isIntersectionPointOnRight(const Point& p, int ind) const;

  public:
      Polygon(const std::vector<Point>& points);
      Polygon(Point* points, int size);
      ~Polygon() { delete[] _points; }

      Point operator[](int ind) const { return _points[convCoord(ind)]; }
      Point& operator[](int ind) { return _points[convCoord(ind)]; }

      const int size() const { return _size; }

      bool isInside(const Point& p) const;
      bool isSimple() const;
      bool isConvex() const;
      int convCoord(int ind) const;

      static Polygon convexHull(const std::vector<Point>& points);
};

#endif
