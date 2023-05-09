#ifndef GEOMETRY_LIB_POLYGON_HPP
#define GEOMETRY_LIB_POLYGON_HPP

#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <iostream>
#include <memory>
#include <vector>

#include "LineSegment.hpp"
#include "Point.hpp"

enum class ConvexHullMethod
{
   GRAHAM, // a.k.a Graham's scan
   JARVIS  // a.k.a gift wrapping algorithm
};
enum class ClipSegmentMethod
{
   COHEN_SUTHERLAND,
   SPROULE_SUTHERLAND,
   CYRUS_BECK
};
class Polygon
{
  private:
   std::vector<Point> _points;

   /**
    * @brief Calculates the count of intersections of the ray and the
    * polygon
    *
    * @param p The point from which the ray originates
    * @param signs Values vector -1, 0, 1. Determine the y-coordinate
    * of a polygon point relative to the y-coordinate of a point p
    * @return Count of intersections
    *
    */
   int static countIntersections(const Point& p,
                                 const std::vector<int>& signs);
   /**
    * @brief Checks if the intersection point is located on the right
    * relative to p?
    *
    */
   int intersectionPointIsOnRight(const Point& p, int ind) const;

  public:
   Polygon(const std::vector<Point>& points);
   Polygon(Point* points, size_t size);
   ~Polygon() {}

   Point operator[](int ind) const { return _points[convCoord(ind)]; }

   /**
    * @brief Get all points of `this` Polygon
    *
    * @return std::vector<Point> points of this polygon
    */
   std::vector<Point> get() const;

   Point& operator[](int ind) { return _points[convCoord(ind)]; }

   const int size() const { return _points.size(); }

   bool isInside(const Point& p) const;
   std::pair<double, const Point*>* anglesForConvexPolygon() const;
   bool isInsideConvexPolygon(
     const Point& p, std::pair<double, const Point*>* angles) const;
   bool isSimple() const;
   bool isConvex() const;
   int convCoord(int ind) const;
   static int convCoord(int ind, int size);

   static bool isInsideTriangle(const Point& p1, const Point& p2,
                                const Point& p3, const Point& p);
   static Polygon convexHull(const std::vector<Point>& points,
                             ConvexHullMethod m);

   /**
    * @brief Cutting a segment using specified method
    *
    * @param ls segment to clip
    * @param m method to use for clip
    *
    * @return std::unique_ptr<LineSegment> pointer to line segment.
    * If the pointer is nullptr, then `ls` is outside the area
    */
   std::unique_ptr<LineSegment> segmentInsidePolygon(
     const LineSegment& ls, ClipSegmentMethod m) const;
};

#endif
