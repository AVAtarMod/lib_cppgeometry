#include "Polygon.hpp"
#include "functions.hpp"

Polygon::Polygon(std::vector<Point> points)
{
    _points = new Point[points.size()] {};
    _size = points.size();
    std::copy_n(points, _size, _points);
}

Polygon::Polygon(Point* points, int size)
{
    _points = new Point[size]{};
    _size = size;
    std::copy_n(points, _size, _points);
}

Point Polygon::operator[](int ind) const
{
    ind %= _size;
    if (ind < 0) ind += _size;
    return _points[ind];
}

Point& Polygon::operator[](int ind)
{
    ind %= _size;
    if (ind < 0) ind += _size;
    return _points[ind];
}

bool Polygon::isInside(const Point& p) const
{
    int s, s1, s2;
    int count_inter_segs = 0;
    int count_inter_points = 0;
    int count_tangents_segs = 0;
    for (int i = 0; i < _size; i++)
    {
        s1 = sign(p["y"] - _points[i]["y"]);
        s2 = sign(p["y"] - _points[i + 1]["y"]);
        if (s1 != s2 || s1 == 0) // There is intersection?
            if (s1 == s2)
            {
                count_tangents_segs++;
                count_inter_points -= 2;
            }
            else if (abs(s1) == abs(s2)) count_inter_segs++;
            else if (s1 == 0)
            {
                s = sign(p["y"] - _points[i - 1]["y"]);
                if (s != s2) count_inter_points++;
            }
            //else 
    }
    return true;
}

bool Polygon::isSimple() const
{
    return true;
}

bool Polygon::isConvex() const
{
    return true;
}

Polygon Polygon::convexHull(const std::vector<Point>& points)
{
    return Polygon(points);
}