#include "Polygon.hpp"
#include "functions.hpp"
#include "Line.hpp"
#include "LineSegment.hpp"

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

int Polygon::isIntersectionPointOnRight(const Point& p, int ind) const
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
            (on_right = isIntersectionPointOnRight(p, i)) >= 0) //on_right value is used later in the code
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

Polygon Polygon::convexHull(const std::vector<Point>& points)
{
   return Polygon(points);
}
