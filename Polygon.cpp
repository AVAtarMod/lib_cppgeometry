#include "Polygon.hpp"

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