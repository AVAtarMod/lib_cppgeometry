#include "Polygon.hpp"
#include "functions.hpp"

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

Point Polygon::operator[](int ind) const
{
   ind %= _size;
   if (ind < 0)
      ind += _size;
   return _points[ind];
}

Point& Polygon::operator[](int ind)
{
   ind %= _size;
   if (ind < 0)
      ind += _size;
   return _points[ind];
}

int Polygon::isIntersection(const Point& p, int s1, int s2, int ind) const
{
    int j;
    int s_buf;
    if (s1 != s2 || s1 == 0) // There is intersection?
        if (s1 == s2) // s1 = 0 s2 = 0
        {
            if (isZero(Point::distance(p, _points[ind]) +
                Point::distance(p, _points[ind + 1]) -
                Point::distance(_points[ind], _points[ind + 1]))) // The point on a horizontal line?
                return 0;
            int _s1 = 0, _s2 = 0;
            for (j = ind + 2; _s1 == 0; j++)
                _s1 = sign(p["y"] - _points[j]["y"]);
            for (j = ind - 1; _s2 == 0; j--)
                _s2 = sign(p["y"] - _points[j]["y"]);
            return isIntersection(p, _s1, _s2, ind);
        }
        else if (abs(s1) == abs(s2)) return 1; // s1 = +-1 s2 = -+1
        else if (s1 == 0) // s1 = 0 s2 = -+1
        {
            s_buf = sign(p["y"] - _points[ind - 1]["y"]);
            if (s_buf != s2 && s_buf != 0) return 1;
        }
        else // s1 = +-1 s2 = 0
        {
            s_buf = sign(p["y"] - _points[ind + 2]["y"]);
            if (s_buf != s1 && s_buf != 0) return 1;
        }
    return -1;
}

bool Polygon::isInside(const Point& p) const
{
    int i, j;
    int s_buf, s1, s2, _s1, _s2;
    int count = 0;
    for (i = 0; i < _size; i++)
        if (_points[i]["x"] >= p["x"] ||
            _points[i + 1]["x"] >= p["x"])
        {
            s1 = sign(p["y"] - _points[i]["y"]);
            s2 = sign(p["y"] - _points[i + 1]["y"]);
            if (s1 != s2 || s1 == 0) // There is intersection?
                if (s1 == s2) // s1 = 0 s2 = 0
                {
                    _s1 = 0;
                    for (j = i + 2; _s1 == 0; j++)
                        _s1 = sign(p["y"] - _points[j]["y"]);
                    _s2 = 0;
                    for (j = i - 1; _s2 == 0; j--)
                        _s2 = sign(p["y"] - _points[j]["y"]);

                    count++;
                }
                else if (abs(s1) == abs(s2)) count++; // s1 = +-1 s2 = -+1
                else if (s1 == 0) // s1 = 0 s2 = -+1
                {
                    s_buf = sign(p["y"] - _points[i - 1]["y"]);
                    if (s_buf != s2 && s_buf != 0) count++;
                }
                else // s1 = +-1 s2 = 0
                {
                    s_buf = sign(p["y"] - _points[i + 1]["y"]);
                    if (s_buf != s1 && s_buf != 0) count++;
                }
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
