#ifndef GEOMETRY_LIB_POINT_HPP
#define GEOMETRY_LIB_POINT_HPP

#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <iostream>

class Point
{
  private:
   double* _coord = nullptr;
   int _size;

   bool is_zeros(int ind) const;
   void zero_normalization();
   bool equal(const Point& a, int size) const;

   friend std::ostream& operator<<(std::ostream& out, const Point& number);

  public:
   Point();
   Point(int size);
   Point(const Point& a);
   Point(double x, double y);
   ~Point() { delete[] _coord; }

   bool operator==(const Point& a) const;
   bool operator!=(const Point& a) const { return !(*this == a); }
   void operator=(const Point& a);
   void operator+=(const Point& a);
   void operator-=(const Point& a);
   Point operator-(const Point& a) const { return (*this) + (-a); }
   Point operator-() const;
   Point operator+(const Point& a) const;
   double operator*(const Point& a) const;
   Point operator^(const Point& a) const;
   /**
    * @brief Returns the third coordinate of the vector resulting from the cross product.
    */
   double operator|(const Point& a) const;
   double operator[](int ind) const { return _coord[ind]; }
   double& operator[](int ind) { return _coord[ind]; }
   double operator[](const char* ch) const;
   double& operator[](const char* ch);

   const int size() const { return _size; }
   const int dimension() const;
   const double length() const { return distance(Point(), *this); }

   void resize(int size);
   void zeroing();
   static double distance(const Point& a, const Point& b);
   double distance(const Point& other) const;
   static int min_size(const Point* arr, int n);
   static int max_size(const Point* arr, int n);
   static Point middle(const Point* arr, int n);
   static Point middle(const Point& a, const Point& b);
   static double cos(const Point& a, const Point& b);
   static double cos(const Point& a, const Point& b, const Point& o);
   static double angle(const Point& a, const Point& b);
   static double angle(const Point& a, const Point& b, const Point& o);
   /**
    * @brief Does check is point has one or both coordinates at infinity
    *
    * @param point A point that need to check
    */
   static bool isAtInfinity(const Point& point);
   static Point zero() { return Point(0.0, 0.0); };
};

std::ostream& operator<<(std::ostream& out, const Point& p);

#endif // GEOMETRY_LIB_POINT_HPP
