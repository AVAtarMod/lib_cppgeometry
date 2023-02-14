#include "Point.hpp"

#include <cmath>

Point::Point(double x, double y)
{
   _coord = new double[2];
   _coord[0] = x;
   _coord[1] = y;
   _size = 2;
}

bool Point::is_zeros(int ind) const
{
    if (ind >= _size) return false;
    for (int i = ind; i < _size; i++)
        if ((*this)[i] != 0) return false;
    return true;
}

void Point::zero_normalization()
{
    int s = Size(), i;
    for (i = s - 1; i > 0 && (*this)[i] == 0; i--)
        s--;
    if (s != Size())
        resize(s);
}

bool Point::equal(const Point& a, int size) const
{
    for (int i = 0; i < size; i++)
        if ((*this)[i] != a[i]) return false;
    return true;
}

Point::Point()
{
    _coord = new double[1]{};
    _size = 1;
}

Point::Point(int size)
{
    _coord = new double[size] {};
    _size = size;
}

Point::Point(double* coord, int size)
{
    _coord = new double[size] {};
    _size = size;
    std::copy_n(coord, size, _coord);
}

Point::Point(const Point& a) { *this = a; }

bool Point::operator==(const Point& a) const
{
    int min = Size();
    if (Size() < a.Size())
    {
        if (!a.is_zeros(a.Size() - Size() - 1)) return false;
    }
    else if (Size() > a.Size())
    {
        min = a.Size();
        if (!is_zeros(Size() - a.Size() - 1)) return false;
    }
    return equal(a, min);
}

void Point::operator = (const Point& a)
{
    this->~Point();
    _coord = new double[a.Size()]{};
    std::copy_n(a._coord, a.Size(), _coord);
    _size = a.Size();
}

void Point::operator += (const Point& a)
{
    if (Size() < a.Size())
        resize(a.Size());
    for (int i = 0; i < Size(); i++)
        (*this)[i] += a[i];
}

void Point::operator -= (const Point& a)
{
    *this += -a;
}

Point Point::operator-() const
{
    Point ans;
    ans = Point(Size());
    for (int i = 0; i < Size(); i++)
        ans[i] = -(*this)[i];
    return ans;
}

Point Point::operator+(const Point& a) const
{
    Point ans = Point();
    ans += *this;
    ans += a;
    return ans;
}

double Point::operator*(const Point& a) const
{
    double ans = 0;
    int min = std::min(a.Size(), Size());
    for (int i = 0; i < min; i++)
        ans += a[i] * (*this)[i];
    return ans;
}

Point Point::operator^(const Point& a) const
{
    Point ans(3);
    Point a1(*this);
    Point a2(a);
    a1.resize(3);
    a2.resize(3);
    ans[0] = a1[1] * a2[2] - a1[2] * a2[1];
    ans[1] = a1[2] * a2[0] - a1[0] * a2[2];
    ans[2] = a1[0] * a2[1] - a1[1] * a2[0];
    return ans;
}

double Point::operator[](char* ch) const
{
    switch (tolower(ch[0]))
    {
    case 'x':
        return (*this)[0];
    case 'y':
        return (*this)[1];
    case 'z':
        return (*this)[2];
    case 'w':
        return (*this)[3];
    }
}

double& Point::operator[](char* ch)
{
    switch (tolower(ch[0]))
    {
    case 'x':
        return (*this)[0];
    case 'y':
        return (*this)[1];
    case 'z':
        return (*this)[2];
    case 'w':
        return (*this)[3];
    }
}

const int Point::Dimension() const
{
    int _dimension = 0;
    for (int i = 0; i < _size; i++)
        if ((*this)[i] != 0) _dimension++;
    return _dimension;
}

void Point::resize(int size)
{
    double* buf = _coord;
    _coord = new double[size] {};
    _size = size;
    std::copy_n(buf, std::min(size, _size), _coord);
    delete[] buf;
}

void Point::zeroing()
{
    this->~Point();
    _coord = new double[1]{};
    _size = 1;
}

double Point::distance(const Point& a, const Point& b)
{
    double ans = 0;
    int i = a.Size();
    if (a.Size() < b.Size())
        for (i = b.Size() - 1; i >= a.Size(); i--)
            ans += b[i] * b[i];
    else if (a.Size() > b.Size())
        for (i = a.Size() - 1; i >= b.Size(); i--)
            ans += a[i] * a[i];
    for (; i >= 0; i--)
        ans += pow(a[i] - b[i], 2);
    return sqrt(ans);
}

int Point::min_size(const Point* arr, int n)
{
    int ans = arr[0].Size();
    for (int i = 1; i < n; i++)
        if (arr[i].Size() < ans) ans = arr[i].Size();
    return ans;
}

int Point::max_size(const Point* arr, int n)
{
    int ans = arr[0].Size();
    for (int i = 1; i < n; i++)
        if (arr[i].Size() > ans) ans = arr[i].Size();
    return ans;
}

Point Point::middle(const Point* arr, int n)
{
    int max = max_size(arr, n);
    Point ans(max);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < arr[i].Size(); j++)
            ans[j] += arr[i][j];
    for (int i = 0; i < max; i++)
        ans[i] /= n;
    return ans;
}

Point Point::middle(const Point& a, const Point& b)
{
    Point arr[2] = { a, b };
    return middle(arr, 2);
    /*Point ans;
    int i = a.Size(), min = a.Size();
    if (a.Size() < b.Size())
    {
        ans = Point(b.Size());
        for (i = ans.Size() - 1; i >= min; i--)
            ans[i] = b[i] / 2;
    }
    else if (a.Size() > b.Size())
    {
        min = b.Size();
        ans = Point(a.Size());
        for (i = ans.Size() - 1; i >= min; i--)
            ans[i] = a[i] / 2;
    }
    for (; i >= 0; i--)
        ans[i] = (a[i] + b[i]) / 2;
    ans.dimension_normalization();
    return ans;*/
}

double Point::angle(const Point& a, const Point& b)
{
    return acos(a * b / (a.Length() * b.Length()));
}

double Point::angle(const Point& a, const Point& b, const Point& o)
{
    return angle(a - o, b - o);
}

bool Point::isAtInfinity(const Point& point)
{
	for (int i = 0; i < point.Size(); i++)
		if (std::isinf(point[i])) return true;
	return false;
}
