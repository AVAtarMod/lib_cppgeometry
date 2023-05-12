#include "Polygon.hpp"
#include "Line.hpp"
#include "functions.hpp"
#include <string>

#include <stdlib.h>
std::unique_ptr<LineSegment> lineClippingCohenSutherland(
  LineSegment ls, const Polygon& polygon);
std::unique_ptr<LineSegment> lineClippingSprouleSutherland(
  LineSegment ls, const Polygon& polygon);
std::unique_ptr<LineSegment> lineClippingCyrusBeck(
  const LineSegment& ls, const Polygon& polygon);

Polygon::Polygon(const std::vector<Point>& points)
{
   _points = points;
}

Polygon::Polygon(Point* points, size_t size)
{
   _points.resize(size);
   for (size_t i = 0; i < size; ++i) {
      _points[i] = points[i];
   }
}

int Polygon::countIntersections(const Point& p,
                                const std::vector<int>& signs)
{
   int count = 0, dif = 0;
   for (int i = 0; i < signs.size() - 1; i++)
      if (signs[i] != signs[i + 1] ||
          signs[i] == 0) // There is intersection?
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
   for (i = 0; i < size() - 1; i++) {
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

std::pair<double, const Point*>* Polygon::anglesForConvexPolygon()
  const
{
   std::pair<double, const Point*>* angles =
     new std::pair<double, const Point*>[size()];
   Point c = Point::middle(_points);
   Point c1 = c + Point(1, 0);
   for (int i = 0; i < size(); i++) {
      angles[i].first = Point::angle((*this)[i], c1, c);
      angles[i].second = &_points[i];
      if ((*this)[i]["y"] < c["y"])
         angles[i].first = -angles[i].first + 2 * M_PI;
   }
   std::sort(angles,
             angles + size(),
             [](std::pair<double, const Point*> a,
                std::pair<double, const Point*>
                  b) { return a.first < b.first; });
   return angles;
}

bool Polygon::isInsideConvexPolygon(
  const Point& p, std::pair<double, const Point*>* angles) const
{
   Point c = Point::middle(_points);
   Point c1 = c + Point(1, 0);
   double angle = Point::angle(p, c1, c);
   if (p["y"] < c["y"])
      angle = -angle + 2 * M_PI;

   int mid = size() / 2, l = 0, r = size() - 1;
   if (angle < angles[0].first || angle > angles[size() - 1].first) {
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
   return Polygon::isInsideTriangle(
     c, *angles[l].second, *angles[r].second, p);
}

bool Polygon::isSimple() const
{
   int i, j;
   for (i = 0; i < size() - 1; i++)
      for (j = i + 2; j < size() - 1; j++)
         if (LineSegment::isIntersection((*this)[i],
                                         (*this)[i + 1],
                                         (*this)[j],
                                         (*this)[j + 1]))
            return false;
   return true;
}

bool Polygon::isConvex() const
{
   int _sign, temp;
   temp = sign(((*this)[1] - (*this)[0]) | ((*this)[2] - (*this)[0]));
   for (int i = 1; i < size() - 1; i++) {
      _sign = temp;
      temp = sign(((*this)[i + 1] - (*this)[i]) |
                  ((*this)[i + 2] - (*this)[i]));
      if (_sign + temp == 0 && _sign != 0)
         return false;
   }
   return true;
}

int Polygon::convCoord(int ind) const
{
   return convCoord(ind, size());
}

int Polygon::convCoord(int ind, int size)
{
   ind %= size;
   if (ind < 0)
      ind += size;
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
   int i;
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

   bool is_zero_angle, is_zero_dis;
   for (i = 0; i < indices.size(); i++) {
      is_zero_angle = isZero(
        points_p[indices[i]][1] -
        points_p[indices[Polygon::convCoord(i + 1, indices.size())]]
                [1]);
      is_zero_dis = isZero(
        points_p[indices[i]][0] -
        points_p[indices[Polygon::convCoord(i + 1, indices.size())]]
                [0]);
      if (is_zero_angle &&
          (is_zero_dis || points_p[indices[i]][0] <
                            points_p[indices[Polygon::convCoord(
                              i + 1, indices.size())]][0])) {
         indices.erase(indices.begin() + i);
         i--;
      }
   }
   double cr_prod;
   int size = indices.size(), j;
   for (i = 0; i <= size; i++) {
      cr_prod =
        (points[indices[Polygon::convCoord(i + 1, indices.size())]] -
         points[indices[Polygon::convCoord(i, indices.size())]]) |
        (points[indices[Polygon::convCoord(i + 2, indices.size())]] -
         points[indices[Polygon::convCoord(i + 1, indices.size())]]);
      if (!isZero(cr_prod) && cr_prod < 0) {
         j = Polygon::convCoord(i + 1, indices.size());
         indices.erase(indices.begin() + j);
         size = indices.size();
         if (j == 0)
            i--;
         i -= 2;
      }
   }
   std::vector<Point> ans(indices.size());
   for (i = 0; i < ans.size(); i++)
      ans[i] = Point(points[indices[i]]);
   return Polygon(ans);
}

struct Side
{
  public:
   enum type
   {
      Left,
      Right
   };
   void operator=(const type& t);
   Side(const type& t);
   static const size_t size = 2;
   type operator+(int shift) const;
   Side operator=(const Side& s) = delete;
   bool operator==(const type& t) const;

  private:
   type _value;
};

Side::type Side::operator+(int shift) const
{
   return static_cast<type>(
     std::abs(static_cast<int>(_value) + shift) % size);
}
void Side::operator=(const type& t)
{
   _value = t;
}
bool Side::operator==(const type& t) const
{
   return t == _value;
}
Side::Side(const type& t)
{
   _value = t;
}

double getValidAngle(const Point& p, const Side& s)
{
   // TODO: fix angle value check (by default angle between -360 and
   // 360, but was between 0 and 360)
   double angle = p.angle360() * (180 / M_PI);
   if (s == Side::Left) {
      if (angle < 0)
         angle += 360;
      angle = 180 - angle;
   }
   if (angle > 180)
      angle = angle - 360;
   // check degree value, throw exception if invalid
   angle = Angle(-90, angle, 90).degrees();
   return angle;
}
std::vector<std::pair<Point, const size_t>> getPointBySide(
  const Side& s, const std::vector<Point>& points)
{
   std::vector<std::pair<Point, const size_t>> result;

   const size_t& size = points.size();
   if (s == Side::Right)
      // Add points which to the right of current
      for (size_t i = 0; i < size; ++i) {
         if (isZero(points[i]["x"]) || points[i]["x"] > 0)
            result.push_back({ points[i], i });
      }
   else if (s == Side::Left)
      // Add points which to the left of current
      for (size_t i = 0; i < size; ++i) {
         if (isZero(points[i]["x"]) || points[i]["x"] < 0)
            result.push_back({ points[i], i });
      }
   return result;
}

/**
 * @brief Get next point by gift wrapping algorithm (next point is
 * point from points vector)
 *
 * @param current current point
 * @param points remained points (all must be not processed and not
 * contain current)
 * @return std::pair<size_t, Point> pair(point index in points, next
 * point)
 */
std::pair<size_t, Point> jarvisGetNextPoint(
  const Point& current, const std::vector<Point>& points, Side& side)
{
   const size_t& size = points.size();
   std::vector<std::pair<Point, const size_t>> view_scope;
   std::vector<Point> normalized(size);

   for (size_t i = 0; i < size; ++i) {
      // Make current (0,0) relative to points
      normalized[i] = points[i] - current;
   }

   view_scope = getPointBySide(side, normalized);
   if (view_scope.size() == 0) {
      side = side + 1;
      view_scope = getPointBySide(side, normalized);
   }

   std::pair<double, size_t> result_angle, cur_angle;
   result_angle = { getValidAngle(view_scope[0].first, side),
                    view_scope[0].second };

   for (size_t i = 1; i < view_scope.size(); ++i) {
      cur_angle.first = getValidAngle(view_scope[i].first, side);
      cur_angle.second =
        view_scope[i].second; // Save index in points to pair

      if (side == Side::Left) {
         if (cur_angle.first > result_angle.first)
            result_angle = cur_angle;
      } else {
         if (cur_angle.first < result_angle.first)
            result_angle = cur_angle;
      }
   }
   size_t result_idx = result_angle.second;
   return { result_idx, points[result_idx] };
}

Polygon jarvisConvexHull(std::vector<Point> points)
{
   const size_t& size = points.size();
   if (size < 3)
      throw std::runtime_error(
        "Cannot construct hull by lesser than 3 points");

   Point min = points[0];
   for (auto&& i : points) {
      if (i["y"] < min["y"])
         min = i;
   }

   std::vector<Point> convexHullPoints;
   convexHullPoints.push_back(min);
   {
      Point current = min;
      size_t i = 0;
      Side s = Side::Right;
      points.erase(std::find(points.begin(), points.end(), current));
      do {
         auto pair = jarvisGetNextPoint(current, points, s);
         current = pair.second;
         convexHullPoints.push_back(current);
         points.erase(points.begin() + pair.first);
         if (i == 0)
            points.push_back(min);
         ++i;
      } while (current != min && points.size() > 0);
   }
   convexHullPoints.pop_back();

   return Polygon(convexHullPoints);
}

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

std::unique_ptr<LineSegment> Polygon::segmentInsidePolygon(
  const LineSegment& s, ClipSegmentMethod m) const
{
   switch (m) {
      case ClipSegmentMethod::COHEN_SUTHERLAND:
         return lineClippingCohenSutherland(s, *this);
      case ClipSegmentMethod::SPROULE_SUTHERLAND:
         return lineClippingSprouleSutherland(s, *this);
      case ClipSegmentMethod::CYRUS_BECK:
         return lineClippingCyrusBeck(s, *this);
      default:
         break;
   }
   return std::unique_ptr<LineSegment>();
}

struct LineEndCode
{
   uint8_t mask = 0b0000;

   LineEndCode(const std::pair<std::pair<double, double>,
                               std::pair<double, double>>& xy_minmax,
               const Point& p)
   {
      const std::pair<double, double>& x = xy_minmax.first;
      const std::pair<double, double>& y = xy_minmax.second;
      if (p["x"] < x.first)
         mask |= 0b0001;
      if (p["x"] > x.second)
         mask |= 0b0010;
      if (p["y"] < y.first)
         mask |= 0b0100;
      if (p["y"] > y.second)
         mask |= 0b1000;
   }
   bool operator==(const LineEndCode& other)
   {
      return mask == other.mask;
   }
};
/**
 * @brief Computes minmax by polygon points
 *
 * @param polygon
 * @return std::pair<std::pair<double, double>, std::pair<double,
 * double>> pair(minmax by X, minmax by Y)
 */
std::pair<std::pair<double, double>, std::pair<double, double>> xy_minmax(
  const Polygon& polygon)
{
   std::pair<double, double> x = { polygon[0]["x"], polygon[0]["x"] },
                             y = { polygon[0]["y"], polygon[0]["y"] };
   for (size_t i = 0; i < polygon.size(); ++i) {
      /**
       * @brief Check current X is min/max
       */
      if (polygon[i]["x"] < x.first)
         x.first = polygon[i]["x"];
      else if (polygon[i]["x"] > x.second)
         x.second = polygon[i]["x"];
      /**
       * @brief Check current Y is min/max
       */
      if (polygon[i]["y"] < y.first)
         y.first = polygon[i]["y"];
      else if (polygon[i]["y"] > y.second)
         y.second = polygon[i]["y"];
   }
   return { x, y };
}

enum class SegmentPosition
{
   UNKNOWN,
   INSIDE,
   OUTSIDE
};
SegmentPosition getSegmentPosition(const LineEndCode& begin,
                                   const LineEndCode& end)
{
   if (begin.mask == end.mask) {
      if (begin.mask == 0)
         return SegmentPosition::INSIDE;
      if ((begin.mask ^ end.mask) == 0)
         return SegmentPosition::OUTSIDE;
   }
   return SegmentPosition::UNKNOWN;
}

/**
 * @brief Get the Point on border is point outise a area
 *
 * @param currentPoint point to move
 * @param x_minmax min and max values of X axis of area
 * @param y_minmax min and max values of Y axis of area
 * @param ls LineSegment on which move point. `currentPoint` must
 * belongs to `ls`
 * @return Point(0) if `currentPoint` already inside area, or
 * valid Point(x,y) if `currentPoint` outside
 */
Point getPointOnBorder(const Point& currentPoint,
                       const std::pair<double, double>& x_minmax,
                       const std::pair<double, double>& y_minmax,
                       const LineSegment& ls)
{
   Point result;
   if (currentPoint["y"] < y_minmax.first) {
      result = ls.getPointByY(y_minmax.first);
   } else if (currentPoint["y"] > y_minmax.second) {
      result = ls.getPointByY(y_minmax.second);
   } else if (currentPoint["x"] < x_minmax.first) {
      result = ls.getPointByX(x_minmax.first);
   } else if (currentPoint["x"] > x_minmax.second) {
      result = ls.getPointByX(x_minmax.second);
   }
   return result;
}

bool isOverlappedEnds(const Point& lsBeginCurrent,
                      const Point& lsEndCurrent,
                      const Point& lsBeginNext,
                      const Point& lsEndNext)
{
   return (sign(lsBeginCurrent["x"] - lsEndCurrent["x"]) !=
             sign(lsBeginNext["x"] - lsEndNext["x"]) ||
           sign(lsBeginCurrent["y"] - lsEndCurrent["y"]) !=
             sign(lsBeginNext["y"] - lsEndNext["y"])) &&
          lsBeginNext != lsEndNext;
}
std::unique_ptr<LineSegment> lineClippingCohenSutherland(
  LineSegment ls, const Polygon& polygon)
{
   if (polygon.size() != 4)
      throw std::runtime_error(
        "Cannot use COHEN_SUTHERLAND method for polygon size != 4 (current size is " +
        std::to_string(polygon.size()) + ")");
   auto ret = xy_minmax(polygon);

   Point lsBeginCurrent = ls.getBegin(), lsEndCurrent = ls.getEnd();
   LineEndCode begin(ret, lsBeginCurrent), end(ret, lsEndCurrent);
   const auto& x = ret.first;
   const auto& y = ret.second;

   SegmentPosition pos = getSegmentPosition(begin, end);
   Point lsBeginNext = lsBeginCurrent, lsEndNext = lsEndCurrent;

   while (pos != SegmentPosition::INSIDE) {
      if (pos == SegmentPosition::OUTSIDE) {
         return std::unique_ptr<LineSegment>();
      }
      /**
       * @brief Store previous value in old variables
       */
      lsBeginCurrent = lsBeginNext, lsEndCurrent = lsEndNext;
      lsBeginNext = getPointOnBorder(lsBeginNext, x, y, ls);
      lsEndNext = getPointOnBorder(lsEndNext, x, y, ls);
#ifdef DEBUG
      std::cout << "DEBUG: prevBegin=" << lsBeginCurrent
                << " prevEnd=" << lsEndCurrent
                << "; begin=" << lsBeginNext << " end=" << lsEndNext
                << "\n";
#endif // DEBUG
      bool isBeginReverted = false, isEndReverted = false;
      if (lsBeginNext == Point()) {
         lsBeginNext = lsBeginCurrent;
         isBeginReverted = true;
      }
      if (lsEndNext == Point()) {
         lsEndNext = lsEndCurrent;
         isEndReverted = true;
      }
      if (!isBeginReverted &&
          isOverlappedEnds(
            lsBeginCurrent, lsEndCurrent, lsBeginNext, lsEndNext)) {
         lsBeginNext = lsBeginCurrent;
      }
      if (!isEndReverted &&
          isOverlappedEnds(
            lsBeginCurrent, lsEndCurrent, lsBeginNext, lsEndNext)) {
         lsEndNext = lsEndCurrent;
      }
      begin = LineEndCode(ret, lsBeginNext),
      end = LineEndCode(ret, lsEndNext);
      pos = getSegmentPosition(begin, end);
   }
   ls = LineSegment(lsBeginNext, lsEndNext);
   return std::make_unique<LineSegment>(ls);
}

/**
 * @brief Get fair LineType.
 *
 * @param l line which type need to compute
 * @return LineType If line slope (value in -90° < x < 90°) is
 * - in 60° < x < 90°,-90 < x < -60 -> CONST_X
 * - in -30° < x  < 30° -> CONST_Y
 * - NORMAL, otherwise.
 */
LineType getFairLineType(const Line& l)
{
   Angle slope = atan(l.K()) * (180 / M_PI);
   if (60 < slope && slope < 90 || -90 < slope && slope < -60) {
      return LineType::CONST_X;
   } else if (-30 < slope && slope < 30) {
      return LineType::CONST_Y;
   }
   return LineType::NORMAL;
}

int8_t getPartSign(LineType fairLineType, const LineSegment& ls)
{
   switch (fairLineType) {
      case LineType::CONST_X:
      case LineType::NORMAL:
         return -sign(ls.getBegin()["y"] - ls.getEnd()["y"]);
         break;
      case LineType::CONST_Y:
         return -sign(ls.getBegin()["x"] - ls.getEnd()["x"]);
         break;
      default:
         break;
   }
   return 0;
}
std::pair<Point, Point> getPartBeginEnd(LineType type, size_t i,
                                        double partLength,
                                        int8_t partSign,
                                        const LineSegment& ls)
{
   std::pair<Point, Point> result;
   double partBegin;
   switch (type) {
      case LineType::CONST_Y:
         partBegin = ls.getBegin()["x"] + i * partLength * partSign;
         result.first = ls.getPointByX(partBegin);
         result.second =
           ls.getPointByX(partBegin + partLength * partSign);
         break;
      case LineType::CONST_X:
      case LineType::NORMAL:
         partBegin = ls.getBegin()["y"] + i * partLength * partSign;
         result.first = ls.getPointByY(partBegin);
         result.second =
           ls.getPointByY(partBegin + partLength * partSign);
         break;
      default:
         break;
   }
   return result;
}
std::unique_ptr<LineSegment> lineClippingSprouleSutherland(
  LineSegment ls, const Polygon& polygon)
{
   if (polygon.size() != 4)
      throw std::runtime_error(
        "Cannot use SPROULE_SUTHERLAND method for polygon size != 4 (current size is " +
        std::to_string(polygon.size()) + ")");

   size_t n = 2;
   const float precision = 0.001;
   const double length = ls.length();

   while (length / n > precision) {
      ++n;
   }

   Point lsBegin = ls.getBegin(), lsEnd = ls.getEnd();
   LineType type = getFairLineType(ls.getLine());
   auto ret = xy_minmax(polygon);
   int8_t partSign = getPartSign(type, ls);
   bool isPartSegmentInside = false;

   SegmentPosition current = SegmentPosition::OUTSIDE, next;
   for (size_t i = 0; i < n; ++i) {
      auto pair = getPartBeginEnd(
        type, i, length / static_cast<double>(n), partSign, ls);
      next = getSegmentPosition(LineEndCode(ret, pair.first),
                                LineEndCode(ret, pair.second));
      if (next == SegmentPosition::INSIDE) {
         isPartSegmentInside = true;
         // May be UNKNOWN too because of border segment
         if (current != SegmentPosition::INSIDE)
            lsBegin = pair.first;
      }
      if (isPartSegmentInside && next != SegmentPosition::INSIDE) {
         lsEnd = pair.second;
         break;
      }
      current = next;
   }
   if (!isPartSegmentInside)
      return std::unique_ptr<LineSegment>(nullptr);

   ls = LineSegment(lsBegin, lsEnd);
   return std::make_unique<LineSegment>(ls);
}

std::unique_ptr<LineSegment> lineClippingCyrusBeck(
  const LineSegment& ls, const Polygon& polygon)
{
   const std::vector<Point> _points = polygon.get();

   Point center = Point::middle(_points);
   int direction =
     sign(_points[1] - _points[0] | center - _points[0]);
   if (direction == 0)
      throw "Polygon is degenerate!";

   Point D(ls.getEnd() - ls.getBegin());
   Point w_i, N_i;
   double Q_i, P_i, t0 = 0, t1 = 1, t;
   for (int i = 0; i != polygon.size() * direction; i += direction) {
      N_i = Point(polygon[i + direction]["y"] - polygon[i]["y"],
                  polygon[i]["x"] - polygon[i + direction]["x"]);
      if (sign(N_i * (center - polygon[i])) == -1)
         N_i = -N_i;
      w_i = Point(ls.getBegin() - polygon[i]);
      Q_i = N_i * w_i;
      P_i = N_i * D;
      if (isZero(P_i)) {
         if (!isZero(Q_i) && Q_i < 0)
            return nullptr;
      } else {
         t = -Q_i / P_i;
         if (isZero(t) || isZero(t - 1) || t > 0 && t < 1)
            if (P_i > 0)
               t0 = t;
            else
               t1 = t;
      }
   }
   double dx, dy;
   dx = ls.getEnd()["x"] - ls.getBegin()["x"];
   dy = ls.getEnd()["y"] - ls.getBegin()["y"];
   return std::unique_ptr<LineSegment>(
     new LineSegment(Point(dx * t0, dy * t0) + ls.getBegin(),
                     Point(dx * t1, dy * t1) + ls.getBegin()));
}

std::vector<Point> Polygon::get() const
{
   return _points;
}
