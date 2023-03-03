#include "Point.hpp"
#include "functions.hpp"

#include <cmath>
#include <limits>

Point::Point(double x, double y)
{
   _coord = new double[2];
   _coord[0] = x;
   _coord[1] = y;
   _size = 2;
}

bool Point::is_zeros(int ind) const
{
   if (ind >= _size)
      return false;
   for (int i = ind; i < _size; i++)
      if ((*this)[i] != 0)
         return false;
   return true;
}

void Point::zero_normalization()
{
   int s = size(), i;
   for (i = s - 1; i > 0 && (*this)[i] == 0; i--)
      s--;
   if (s != size())
      resize(s);
}

bool Point::equal(const Point& a, int size) const
{
   for (int i = 0; i < size; i++)
      if (fabs((*this)[i] - a[i]) > std::numeric_limits<double>::epsilon())
         return false;
   return true;
}

Point::Point()
{
   _coord = new double[1] {};
   _size = 1;
}

Point::Point(int size)
{
   _coord = new double[size] {};
   _size = size;
}

Point::Point(const Point& a)
{
   *this = a;
}

bool Point::operator==(const Point& a) const
{
   int min = size();
   if (size() < a.size()) {
      if (!a.is_zeros(a.size() - size() - 1))
         return false;
   } else if (size() > a.size()) {
      min = a.size();
      if (!is_zeros(size() - a.size() - 1))
         return false;
   }
   return equal(a, min);
}

void Point::operator=(const Point& a)
{
   this->~Point();
   int size = a.size();
   _coord = new double[size] {};
   std::copy_n(a._coord, size, _coord);
   _size = size;
}

void Point::operator+=(const Point& a)
{
   if (size() < a.size())
      resize(a.size());
   for (int i = 0; i < size(); i++)
      (*this)[i] += a[i];
}

void Point::operator-=(const Point& a)
{
   *this += -a;
}

Point Point::operator-() const
{
   Point ans;
   ans = Point(size());
   for (int i = 0; i < size(); i++)
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
   int min = std::min(a.size(), size());
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

double Point::operator|(const Point& a) const
{
   return (*this)[0] * a[1] - (*this)[1] * a[0];
}

double Point::operator[](const char* ch) const
{
   switch (tolower(ch[0])) {
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

double& Point::operator[](const char* ch)
{
   switch (tolower(ch[0])) {
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

const int Point::dimension() const
{
   int _dimension = 0;
   for (int i = 0; i < _size; i++)
      if ((*this)[i] != 0)
         _dimension++;
   return _dimension;
}

void Point::resize(int size)
{
   double* buf = _coord;
   _coord = new double[size] {};
   std::copy_n(buf, std::min(size, _size), _coord);
   _size = size;
   delete[] buf;
}

void Point::zeroing()
{
   this->~Point();
   _coord = new double[1] {};
   _size = 1;
}

double Point::distance(const Point& a, const Point& b)
{
   double ans = 0;
   int i = a.size() - 1;
   if (a.size() < b.size())
      for (i = b.size() - 1; i >= a.size(); i--)
         ans += b[i] * b[i];
   else if (a.size() > b.size())
      for (i = a.size() - 1; i >= b.size(); i--)
         ans += a[i] * a[i];
   for (; i >= 0; i--)
      ans += pow(a[i] - b[i], 2);
   return sqrt(ans);
}

double Point::distance(const Point& other) const
{
   return Point::distance(*this, other);
}

int Point::min_size(const Point* arr, int n)
{
   int ans = arr[0].size();
   for (int i = 1; i < n; i++)
      if (arr[i].size() < ans)
         ans = arr[i].size();
   return ans;
}

int Point::max_size(const Point* arr, int n)
{
   int ans = arr[0].size();
   for (int i = 1; i < n; i++)
      if (arr[i].size() > ans)
         ans = arr[i].size();
   return ans;
}

Point Point::middle(const Point* arr, int n)
{
   int max = max_size(arr, n);
   Point ans(max);
   for (int i = 0; i < n; i++)
      for (int j = 0; j < arr[i].size(); j++)
         ans[j] += arr[i][j];
   for (int i = 0; i < max; i++)
      ans[i] /= n;
   return ans;
}

Point Point::middle(const Point& a, const Point& b)
{
   Point arr[2] = { a, b };
   return middle(arr, 2);
}

double Point::cos(const Point& a, const Point& b)
{
   double _cos = a * b / (a.length() * b.length());
   if (_cos > 1)
      _cos = 1;
   else if (_cos < -1)
      _cos = -1;
   return _cos;
}

double Point::cos(const Point& a, const Point& b, const Point& o)
{
   return cos(a - o, b - o);
}

double Point::angle(const Point& a, const Point& b)
{
   return acos(cos(a, b));
}

double Point::angle(const Point& a, const Point& b, const Point& o)
{
   return angle(a - o, b - o);
}

Point Point::getRandom(int min, int max, size_t size)
{
   Point result(size);
   for (uint i = 0; i < size; ++i) {
      result[i] = getPRNFast(min, max);
   }
   return result;
}

bool Point::isAtInfinity(const Point& point)
{
   for (int i = 0; i < point.size(); i++)
      if (std::isinf(point[i]))
         return true;
   return false;
}

std::ostream& operator<<(std::ostream& out, const Point& p)
{
   size_t size = p.size();
   for (size_t i = 0; i < size - 1; ++i) {
      out << p[i] << " ";
   }
   out << p[size - 1];
   return out;
}

Angle Point::angleDegrees(const Point& a, const Point& b)
{
   return angle(a, b) * (180 / M_PI);
}
Angle Point::angleDegrees(const Point& a, const Point& o, const Point& b)
{
   return angle(a, b, o) * (180 / M_PI);
}
