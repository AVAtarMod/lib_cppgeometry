#ifndef GEOMETRY_LIB_CURVE_HPP
#define GEOMETRY_LIB_CURVE_HPP

#include <vector>

#include "Point.hpp"

class Curve
{
  private:
   std::vector<Point> _points;

  public:
   Curve(const std::vector<Point> points);
   ~Curve();

   size_t size() { return _points.size(); }
   const Point& operator[](size_t index) const
   {
      return _points.at(index);
   }

   /**
    * @brief Make quadratic bézier curve
    *
    * @param P0 Start point
    * @param P1 Point in the middle
    * @param P2 End point
    * @return Curve
    */
   static Curve makeBezierCurve(const Point& P0, const Point& P1,
                                const Point& P2);
   /**
    * @brief Make cubic bézier curve
    *
    * @param P0 Start point
    * @param P1 Point in the middle
    * @param P2 End point
    * @return Curve
    */
   static Curve makeBezierCurve(const Point& P0, const Point& P1,
                                const Point& P2, const Point& P3);
};

#endif // GEOMETRY_LIB_CURVE_HPP
