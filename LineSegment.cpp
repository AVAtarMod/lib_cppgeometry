#include "LineSegment.hpp"

#include "functions.hpp"
#include <cmath>
#include <cstdint>

#include <set>

LineSegment::LineSegment()
{
   _line = Line();
   _endpoints[0] = Point();
   _endpoints[1] = Point();
}

LineSegment::LineSegment(const Point& a, const Point& b) :
  _endpoints { a, b }
{
   if (_endpoints[0] != _endpoints[1])
      _line = Line(a, b);
}

LineSegment::LineSegment(const Line& l, const Point endpoints[2]) :
  _line(l), _endpoints { endpoints[0], endpoints[1] }
{
   bool isPointsCorrect =
     l.isBelongs(endpoints[0]) && l.isBelongs(endpoints[1]);
   if (!isPointsCorrect)
      throw std::runtime_error(
        "Cannot construct line segment: endpoints not on source line");
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
Line LineSegment::getLine() const
{
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
        "LineSegment::move: one of argument endpoints must be endpoint of this segment");

   double dx, dy;
   dx = otherPoints[otherIdx]["x"] - _endpoints[thisIdx]["x"];
   dy = otherPoints[otherIdx]["y"] - _endpoints[thisIdx]["y"];

   Point movePoint(dx, dy);
   return LineSegment(_endpoints[0] + movePoint,
                      _endpoints[1] + movePoint);
}

LineSegment::~LineSegment()
{
   //! There is no need manual cleanup of points and line
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

struct event
{
   double x;
   int tp, id;

   event() {}
   event(double x, int tp, int id) : x(x), tp(tp), id(id) {}

   bool operator<(const event& e) const
   {
      if (abs(x - e.x) > eps)
         return x < e.x;
      return tp > e.tp;
   }
};

bool operator<(const LineSegment& a, const LineSegment& b)
{
   double x = std::max(std::min(a.getBegin()["x"], a.getEnd()["x"]),
                       std::min(b.getBegin()["x"], b.getEnd()["x"]));
   return a.getLine().y(x) < b.getLine().y(x) - eps;
}

inline std::set<LineSegment>::iterator prev(
  std::set<LineSegment>::iterator it, const std::set<LineSegment>& s)
{
   return it == s.begin() ? s.end() : --it;
}

inline std::set<LineSegment>::iterator next(
  std::set<LineSegment>::iterator it)
{
   return ++it;
}

bool LineSegment::isIntersection(const std::vector<LineSegment>& vec)
{
   int n = vec.size();
   std::vector<event> e;
   for (int i = 0; i < n; ++i) {
      e.push_back(
        event(std::min(vec[i].getBegin()["x"], vec[i].getEnd()["x"]),
              +1,
              i));
      e.push_back(
        event(std::max(vec[i].getBegin()["x"], vec[i].getEnd()["x"]),
              -1,
              i));
   }
   std::sort(e.begin(), e.end());

   std::set<LineSegment> s = std::set<LineSegment>();
   std::vector<std::set<LineSegment>::iterator> iters(n);

   for (size_t i = 0; i < e.size(); ++i) {
      int id = e[i].id;
      if (e[i].tp == +1) {
         std::set<LineSegment>::iterator nxt = s.lower_bound(vec[id]),
                                         prv = prev(nxt, s);
         if (nxt != s.end() && (*nxt).isIntersection(vec[id]))
            return true;
         if (prv != s.end() && (*prv).isIntersection(vec[id]))
            return true;
         iters[id] = s.insert(nxt, vec[id]);
      } else {
         std::set<LineSegment>::iterator nxt = next(iters[id]),
                                         prv = prev(iters[id], s);
         if (nxt != s.end() && prv != s.end() &&
             (*nxt).isIntersection(*prv))
            return true;
         s.erase(iters[id]);
      }
   }
   return false;
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
   auto minmax = std::minmax(_endpoints[0]["x"], _endpoints[1]["x"]);
   if (x < minmax.first || x > minmax.second)
      throw std::invalid_argument(
        "parameter 'x' outside segment values range.");
   return Point(x, _line.y(x));
}
Point LineSegment::getPointByY(double y) const
{
   auto minmax = std::minmax(_endpoints[0]["y"], _endpoints[1]["y"]);
   if (y < minmax.first || y > minmax.second)
      throw std::invalid_argument(
        "parameter 'y' outside segment values range.");
   return Point(_line.x(y), y);
}
