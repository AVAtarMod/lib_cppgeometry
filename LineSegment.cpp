#include "LineSegment.hpp"

#include "functions.hpp"
#include "tfunctions.hpp"
#include <cmath>

LineSegment::LineSegment(const Point& a, const Point& b) :
  _line(a, b), _endpoints { a, b }
{
}

LineSegment::LineSegment(const Line& l, const Point endpoints[2]) :
  _line(l), _endpoints { endpoints[0], endpoints[1] }
{
   bool isPointsCorrect =
     l.isBelongs(endpoints[0]) && l.isBelongs(endpoints[1]);
   if (!isPointsCorrect)
      throw std::runtime_error(
        "Cannot construct line segment: endpoints not on soource line");
}

std::pair< Point, Point > LineSegment::getEndpoints() const
{
   return std::pair< Point, Point > { _endpoints[0], _endpoints[1] };
}
double LineSegment::length() const
{
   // ((a_x-b_x)^2+(a_y-b_y)^2)^(1/2)
   return sqrt(power(_endpoints[1].X() - _endpoints[0].X(), 2) +
               power(_endpoints[1].Y() - _endpoints[0].Y(), 2));
}

LineSegment LineSegment::move(const LineSegment& other) const
{
   const std::pair< Point, Point >& endpoints = other.getEndpoints();
   const Point otherPoints[2] { endpoints.first, endpoints.second };

   uint8_t thisIdx = 0, otherIdx = 0;
   if (_endpoints[0] == endpoints.first) {
      otherIdx = 1;
   } else if (_endpoints[1] == endpoints.first) {
      otherIdx = 1;
      thisIdx = 1;
   } else if (_endpoints[1] == endpoints.second) {
      otherIdx = 0;
      thisIdx = 1;
   } else
      throw std::invalid_argument(
        "LineSegment::move: one of argument endpoints must be enpoint of this segment");

   double dx, dy;
   dx = otherPoints[otherIdx].X() - _endpoints[thisIdx].X();
   dy = otherPoints[otherIdx].Y() - _endpoints[thisIdx].Y();
   
   Point movePoint(dx, dy);
   return LineSegment(_endpoints[0] + movePoint, _endpoints[1] + movePoint);
}

LineSegment::~LineSegment()
{
   /**
    * @brief Cleanup of points and line
    */
   _line.~Line();
   _endpoints[0].~Point();
   _endpoints[1].~Point();
}
