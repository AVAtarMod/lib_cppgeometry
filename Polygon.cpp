#include "Polygon.hpp"

Polygon::Polygon(std::vector<Point> points)
{
    _points = new Point[points.size()] {};
    _size = points.size();
    std::copy_n(points, _size, _points);
}

Polygon::Polygon(double* points, int size)
{
    _points = new Point[size]{};
    _size = size;
    std::copy_n(points, _size, _points);
}

bool Polygon::isInside(const Point& p)
{

}

bool Polygon::isSimple()
{

}

bool Polygon::isConvex()
{

}

Polygon Polygon::convexHull(std::vector<Point> points)
{

}