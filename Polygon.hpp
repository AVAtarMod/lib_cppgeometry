#ifndef GEOMETRY_LIB_POLYGON_HPP
#define GEOMETRY_LIB_POLYGON_HPP

#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <iostream>
#include <vector>

#include "Point.hpp"
#include "LineSegment.hpp"

enum class ConvexHullMethod
{
   GRAHAM, // a.k.a Graham's scan
   JARVIS  // a.k.a gift wrapping algorithm
};
class Polygon
{
  private:
   Point* _points = nullptr;
   size_t _size;

   /**
    * @brief Calculates the count of intersections of the ray and the polygon
    *
    * @param p The point from which the ray originates
    * @param signs Values vector -1, 0, 1. Determine the y-coordinate of a
    * polygon point relative to the y-coordinate of a point p
    * @return Count of intersections
    *
    */
   int static countIntersections(const Point& p, const std::vector<int>& signs);
   /**
    * @brief Checks if the intersection point is located on the right relative
    * to p?
    *
    */
   int intersectionPointIsOnRight(const Point& p, int ind) const;

  public:
   Polygon(const std::vector<Point>& points);
   Polygon(Point* points, int size);
   ~Polygon() { delete[] _points; }

   Point operator[](int ind) const { return _points[convCoord(ind)]; }

   /**
    * @brief Get all points of `this` Polygon
    * 
    * @return std::vector<Point> points of this polygon
    */
   std::vector<Point> get() const;

   Point& operator[](int ind) { return _points[convCoord(ind)]; }

   const int size() const { return _size; }

   bool isInside(const Point& p) const;
   std::pair<double, const Point*>* anglesForConvexPolygon() const;
   bool isInsideConvexPolygon(const Point& p,
                              std::pair<double, const Point*>* angles) const;
   bool isSimple() const;
   bool isConvex() const;
   int convCoord(int ind) const;
   static int convCoord(int ind, int size);

   static bool isInsideTriangle(const Point& p1, const Point& p2,
                                const Point& p3, const Point& p);
   static Polygon convexHull(const std::vector<Point>& points,
                             ConvexHullMethod m);
   LineSegment* LineClippingCyrusBeck(const LineSegment& ls) const;
};

#endif
