#include "Polygon.hpp"
#include "Line.hpp"
#include "LineSegment.hpp"
#include "functions.hpp"
#include <stdlib.h>

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

int Polygon::countIntersections(const Point& p, const std::vector<int>& signs)
{
   int count = 0, dif = 0;
   for (int i = 0; i < signs.size() - 1; i++)
      if (signs[i] != signs[i + 1] || signs[i] == 0) // There is intersection?
         if (signs[i] == signs[i + 1])
            dif++; // signs[i] = 0 signs[i + 1] = 0
         else if (abs(signs[i]) == abs(signs[i + 1]))
            count++;             // signs[i] = +-1 signs[i + 1] = -+1
         else if (signs[i] == 0) // signs[i] = 0 signs[i + 1] = -+1
         {
            if (signs[i - dif] != signs[i + 1])
               count++; // signs[i - dif] = +-1 signs[i + 1] = -+1
            dif = 0;
         } else
            dif++; // signs[i] = +-1 signs[i + 1] = 0
   return count;
}

int Polygon::intersectionPointIsOnRight(const Point& p, int ind) const
{
   Point inter = Line::intersect(Line((*this)[ind], (*this)[ind + 1]),
                                 Line(p, Point(p["x"] + 1, p["y"])));
   if (std::isinf(inter["x"])) {
      if (LineSegment::isBelongs((*this)[ind], (*this)[ind + 1], p))
         return 0;
   } else if (p == inter)
      return 0;
   if (0 >= p["x"] - inter["x"])
      return 1;
   else
      return -1;
}

bool Polygon::isInside(const Point& p) const
{
   int i, on_right = -1;
   bool cond_i, cond_i_1, is_part1 = true;
   std::vector<int> signs_part1 = std::vector<int>();
   std::vector<int> signs_part2 = std::vector<int>();

   cond_i_1 = (*this)[0]["x"] >= p["x"];
   for (i = 0; i < _size - 1; i++) {
      cond_i = cond_i_1;
      cond_i_1 = (*this)[i + 1]["x"] >= p["x"];
      if ((cond_i && cond_i_1) ||
          (cond_i || cond_i_1) &&
            (on_right = intersectionPointIsOnRight(p, i)) >=
              0) // on_right value is used later in the code
      {
         if (on_right == 0)
            return true;
         if (is_part1) {
            if (signs_part1.size() == 0)
               signs_part1.push_back(sign(p["y"] - (*this)[i]["y"]));
            signs_part1.push_back(sign(p["y"] - (*this)[i + 1]["y"]));
         } else {
            if (signs_part2.size() == 0)
               signs_part2.push_back(sign(p["y"] - (*this)[i]["y"]));
            signs_part2.push_back(sign(p["y"] - (*this)[i + 1]["y"]));
         }
      } else
         is_part1 = false;
   }

   for (i = 0; i < signs_part1.size(); i++)
      signs_part2.push_back(signs_part1[i]);
   if (countIntersections(p, signs_part2) % 2 == 0)
      return false;
   else
      return true;
}

std::pair<double, const Point*>* Polygon::anglesForConvexPolygon() const
{
   std::pair<double, const Point*>* angles =
     new std::pair<double, const Point*>[_size];
   Point c = Point::middle(_points, 3);
   Point c1 = c + Point(1, 0);
   for (int i = 0; i < _size; i++) {
      angles[i].first = Point::angle((*this)[i], c1, c);
      angles[i].second = &_points[i];
      if ((*this)[i]["y"] < c["y"])
         angles[i].first = -angles[i].first + 2 * M_PI;
   }
   std::sort(
     angles,
     angles + _size,
     [](std::pair<double, const Point*> a, std::pair<double, const Point*> b) {
        return a.first < b.first;
     });
   return angles;
}

bool Polygon::isInsideConvexPolygon(
  const Point& p, std::pair<double, const Point*>* angles) const
{
   Point c = Point::middle(_points, 3);
   Point c1 = c + Point(1, 0);
   double angle = Point::angle(p, c1, c);
   if (p["y"] < c["y"])
      angle = -angle + 2 * M_PI;

   int mid = _size / 2, l = 0, r = _size - 1;
   if (angle < angles[0].first || angle > angles[_size - 1].first) {
      l = r;
      r = 0;
   } else
      while (r - l != 1) {
         if (angle < angles[mid].first)
            r = mid;
         else
            l = mid;
         mid = (l + r) / 2;
      }
   return Polygon::isInsideTriangle(c, *angles[l].second, *angles[r].second, p);
}

bool Polygon::isSimple() const
{
   int i, j;
   for (i = 0; i < _size - 1; i++)
      for (j = i + 2; j < _size - 1; j++)
         if (LineSegment::isIntersection(
               (*this)[i], (*this)[i + 1], (*this)[j], (*this)[j + 1]))
            return false;
   return true;
}

bool Polygon::isConvex() const
{
   int _sign, temp;
   temp = sign(((*this)[1] - (*this)[0]) | ((*this)[2] - (*this)[0]));
   for (int i = 1; i < _size - 1; i++) {
      _sign = temp;
      temp =
        sign(((*this)[i + 1] - (*this)[i]) | ((*this)[i + 2] - (*this)[i]));
      if (_sign + temp == 0 && _sign != 0)
         return false;
   }
   return true;
}

int Polygon::convCoord(int ind) const
{
   ind %= _size;
   if (ind < 0)
      ind += _size;
   return ind;
}

bool Polygon::isInsideTriangle(const Point& p1, const Point& p2,
                               const Point& p3, const Point& p)
{
   int sum = 0;
   sum += sign((p1 - p3) | (p - p3));
   sum += sign((p2 - p1) | (p - p1));
   sum += sign((p3 - p2) | (p - p2));
   sum = abs(sum);
   if (sum == 3 || sum == 2)
      return true;
   else if (LineSegment::isBelongs(p1, p2, p) ||
            LineSegment::isBelongs(p2, p3, p) ||
            LineSegment::isBelongs(p3, p1, p))
      return true;
   else
      return false;
}

Polygon grahamConvexHull(const std::vector<Point>& points)
{
   int i, j;
   Point o = Point::middle(&points[0], points.size());
   std::vector<Point> points_p(points);
   for (i = 0; i < points_p.size(); i++)
      points_p[i].toPolarCoord2(o);

   std::vector<int> indices(points.size());
   for (i = 0; i < indices.size(); i++)
      indices[i] = i;
   std::sort(indices.begin(), indices.end(), [&](int a, int b) {
      return points_p[a][1] < points_p[b][1];
   });

   bool is_zero;
   for (i = 0; i < indices.size(); i++) {
      is_zero = isZero(points_p[indices[i]][1] -
          points_p[indices[i + 1]][1]);
      if (is_zero || points_p[indices[i]][0] <
          points_p[indices[i + 1]][0]) {
         indices.erase(indices.begin() + i);
         i--;
      }
   }

   double cr_prod;
   for (i = 0; i <= indices.size(); i++) {
      cr_prod = points[indices[i + 1]] - points[indices[i]] |
                points[indices[i + 2]] - points[indices[i + 1]];
      if (!isZero(cr_prod) && cr_prod < 0) {
         indices.erase(indices.begin() + i + 1);
         i -= 2;
      }
   }

   std::vector<Point> ans(indices.size());
   for (i = 0; i < ans.size(); i++)
      ans[i] = Point(points[indices[i]]);
   return Polygon(ans);
}

Polygon jarvisConvexHull(const std::vector<Point>& points) {}

Polygon Polygon::convexHull(const std::vector<Point>& points,
                            ConvexHullMethod m)
{
   switch (m) {
      case ConvexHullMethod::GRAHAM:
         return grahamConvexHull(points);
      case ConvexHullMethod::JARVIS:
         return jarvisConvexHull(points);
      default:
         break;
   }
   return Polygon(points);
}
