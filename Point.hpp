#ifndef GEOMETRY_LIB_POINT_HPP
#define GEOMETRY_LIB_POINT_HPP

#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <iostream>

#include "Angle.hpp"

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
    * @brief Returns the third coordinate of the vector resulting from the cross
    * product.
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
    * @brief Compute angle AOB, where O = Point::zero()
    *
    * @return Angle (in degrees)
    */
   static Angle angleDegrees(const Point& a, const Point& b);
   /**
    * @brief Compute angle AOB
    *
    * @param a First Point
    * @param o Middle Point
    * @param b Last Point
    * @return Angle (in degrees)
    */
   static Angle angleDegrees(const Point& a, const Point& o, const Point& b);
   /**
    * @brief Does check is point has one or both coordinates at infinity
    *
    * @param point A point that need to check
    */
   static bool isAtInfinity(const Point& point);
   static Point zero() { return Point(0.0, 0.0); };
   /**
    * @brief Get the Point object with random coordinates
    *
    * @param min lower limit for random coordinates
    * @param max upper limit for random coordinates
    * @param size size of result Point. Exmaple: with size=2 Point will be 2D.
    * @return Point
    */
   static Point getRandom(int min, int max, size_t size = 2);
};

std::ostream& operator<<(std::ostream& out, const Point& p);

#endif // GEOMETRY_LIB_POINT_HPP
