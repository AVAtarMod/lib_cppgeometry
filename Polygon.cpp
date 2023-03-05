#include "Polygon.hpp"
#include "functions.hpp"
#include "Line.hpp"
#include "LineSegment.hpp"
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
            if (signs[i] == signs[i + 1]) dif++; // signs[i] = 0 signs[i + 1] = 0
            else if (abs(signs[i]) == abs(signs[i + 1])) count++; // signs[i] = +-1 signs[i + 1] = -+1
            else if (signs[i] == 0) // signs[i] = 0 signs[i + 1] = -+1
            {
                if (signs[i - dif] != signs[i + 1]) count++; // signs[i - dif] = +-1 signs[i + 1] = -+1
                dif = 0;
            }
            else dif++; // signs[i] = +-1 signs[i + 1] = 0
    return count;
}

int Polygon::intersectionPointIsOnRight(const Point& p, int ind) const
{
    Point inter = Line::intersect(
        Line((*this)[ind], (*this)[ind + 1]),
        Line(p, Point(p["x"] + 1, p["y"])));
    if (std::isinf(inter["x"]))
    {
        if (LineSegment::isBelongs((*this)[ind], (*this)[ind + 1], p))
            return 0;
    }
    else if (p == inter) return 0;
    if (0 >= p["x"] - inter["x"]) return 1;
    else return -1;
}

bool Polygon::isInside(const Point& p) const
{
    int i, on_right = -1;
    bool cond_i, cond_i_1, is_part1 = true;
    std::vector<int> signs_part1 = std::vector<int>();
    std::vector<int> signs_part2 = std::vector<int>();

    cond_i_1 = (*this)[0]["x"] >= p["x"];
    for (i = 0; i < _size - 1; i++)
    {
        cond_i = cond_i_1;
        cond_i_1 = (*this)[i + 1]["x"] >= p["x"];
        if ((cond_i && cond_i_1) ||
            (cond_i || cond_i_1) &&
            (on_right = intersectionPointIsOnRight(p, i)) >= 0) //on_right value is used later in the code
        {
            if (on_right == 0) return true;
            if (is_part1)
            {
                if (signs_part1.size() == 0)
                    signs_part1.push_back(sign(p["y"] - (*this)[i]["y"]));
                signs_part1.push_back(sign(p["y"] - (*this)[i + 1]["y"]));
            }
            else
            {
                if (signs_part2.size() == 0)
                    signs_part2.push_back(sign(p["y"] - (*this)[i]["y"]));
                signs_part2.push_back(sign(p["y"] - (*this)[i + 1]["y"]));
            }
        }
        else is_part1 = false;
    }

    for (i = 0; i < signs_part1.size(); i++)
        signs_part2.push_back(signs_part1[i]);
    if (countIntersections(p, signs_part2) % 2 == 0)
        return false;
    else return true;
}

std::pair<double, const Point*>* Polygon::anglesForConvexPolygon() const
{
    std::pair<double, const Point*>* angles = new std::pair<double, const Point*>[_size];
    Point c = Point::middle(_points, 3);
    Point c1 = c + Point(1, 0);
    for (int i = 0; i < _size; i++)
    {
        angles[i].first = Point::angle((*this)[i], c1, c);
        angles[i].second = &_points[i];
        if ((*this)[i]["y"] < c["y"]) angles[i].first = -angles[i].first + 2 * M_PI;
    }
    std::sort(angles, angles + _size,
        [](std::pair<double, const Point*> a,
        std::pair<double, const Point*> b)
        { return a.first < b.first; });
    return angles;
}

bool Polygon::isInsideConvexPolygon(const Point& p, std::pair<double, const Point*>* angles) const
{
    Point c = Point::middle(_points, 3);
    Point c1 = c + Point(1, 0);
    double angle = Point::angle(p, c1, c);
    if (p["y"] < c["y"]) angle = -angle + 2 * M_PI;

    int mid = _size / 2, l = 0, r = _size - 1;
    if (angle < angles[0].first || angle > angles[_size - 1].first)
    {
        l = r;
        r = 0;
    }
    else while (r - l != 1)
    {
        if (angle < angles[mid].first) r = mid;
        else l = mid;
        mid = (l + r) / 2;
    }
    return Polygon::isInsideTriangle(c,
        *angles[l].second, *angles[r].second, p);
}

bool Polygon::isSimple() const
{
   return true;
}

bool Polygon::isConvex() const
{
   return true;
}

int Polygon::convCoord(int ind) const
{
    ind %= _size;
    if (ind < 0)
        ind += _size;
    return ind;
}

bool Polygon::isInsideTriangle(const Point& p1, const Point& p2, const Point& p3, const Point& p)
{
    int sum = 0;
    sum += sign((p1 - p3) | (p - p3));
    sum += sign((p2 - p1) | (p - p1));
    sum += sign((p3 - p2) | (p - p2));
    sum = abs(sum);
    if (sum == 3 || sum == 2) return true;
    else if (LineSegment::isBelongs(p1, p2, p) ||
        LineSegment::isBelongs(p2, p3, p) ||
        LineSegment::isBelongs(p3, p1, p))
        return true;
    else return false;
}

Polygon Polygon::convexHull(const std::vector<Point>& points)
{
   return Polygon(points);
}
