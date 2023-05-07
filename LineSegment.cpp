#include "LineSegment.hpp"

#include "functions.hpp"
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

std::pair<Point, Point> LineSegment::getEndpoints() const
{
   return std::pair<Point, Point> { _endpoints[0], _endpoints[1] };
}
Point LineSegment::getBegin() const
{
   return _endpoints[0];
}
Point LineSegment::getEnd() const
{
   return _endpoints[1];
}
Line LineSegment::getLine() const {
   return _line;
}
double LineSegment::length() const
{
   // ((a_x-b_x)^2+(a_y-b_y)^2)^(1/2)
   return sqrt(power(_endpoints[1]["x"] - _endpoints[0]["x"], 2) +
               power(_endpoints[1]["y"] - _endpoints[0]["y"], 2));
}

LineSegment LineSegment::move(const LineSegment& other) const
{
   const std::pair<Point, Point>& endpoints = other.getEndpoints();
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
   dx = otherPoints[otherIdx]["x"] - _endpoints[thisIdx]["x"];
   dy = otherPoints[otherIdx]["y"] - _endpoints[thisIdx]["y"];

   Point movePoint(dx, dy);
   return LineSegment(_endpoints[0] + movePoint,
                      _endpoints[1] + movePoint);
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

bool LineSegment::isIntersection(const Point& p1, const Point& p2,
                                 const Point& p3, const Point& p4)
{
   bool c1 = LineSegment::isBelongs(p3, p4, p1);
   bool c2 = LineSegment::isBelongs(p3, p4, p2);
   bool c3 = LineSegment::isBelongs(p1, p2, p3);
   bool c4 = LineSegment::isBelongs(p1, p2, p4);
   if (c1 || c2 || c3 || c4)
      return true;

   double cros_prod1 = (p3 - p1) | (p2 - p1);
   double cros_prod2 = (p2 - p1) | (p4 - p1);
   double cros_prod3 = (p1 - p3) | (p4 - p3);
   double cros_prod4 = (p4 - p3) | (p2 - p3);
   if ((sign(cros_prod1) == sign(cros_prod2)) ||
       (sign(cros_prod3) == sign(cros_prod4)))
      return true;
   return false;
}

bool LineSegment::isIntersection(const LineSegment& ls) const
{
   std::pair<Point, Point> p1 { _endpoints[0], _endpoints[1] };
   std::pair<Point, Point> p2 { ls._endpoints[0], ls._endpoints[1] };
   return LineSegment::isIntersection(
     p1.first, p1.second, p2.first, p2.second);
}

bool LineSegment::isBelongs(const Point& p1, const Point& p2,
                            const Point& p)
{
   return isZero(Point::distance(p, p1) + Point::distance(p, p2) -
                 Point::distance(p1, p2));
}

bool LineSegment::isBelongs(const Point& p) const
{
   return isBelongs(this->_endpoints[0], this->_endpoints[1], p);
}

Point LineSegment::getPointByX(double x) const
{
   return Point(x, _line.y(x));
}
Point LineSegment::getPointByY(double y) const
{
   return Point(_line.x(y), y);
}
