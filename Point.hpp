#ifndef GEOMETRY_LIB_POINT_HPP
#define GEOMETRY_LIB_POINT_HPP

#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <iostream>
#include <vector>

#include "Angle.hpp"

class Point
{
  private:
   std::vector<double> _coordinates;

   bool is_zeros(int ind) const;
   void zero_normalization();
   bool equal(const Point& a, int size) const;

   friend std::ostream& operator<<(std::ostream& out,
                                   const Point& number);

  public:
   Point();
   Point(int size);
   Point(const Point& a);
   Point(double x, double y);
   ~Point() {}

   bool operator==(const Point& a) const;
   bool operator!=(const Point& a) const { return !(*this == a); }
   void operator=(const Point& a);
   void operator+=(const Point& a);
   void operator-=(const Point& a);
   Point operator-(const Point& a) const { return *this + (-a); }
   Point operator-() const;
   Point operator+(const Point& a) const;
   double operator*(const Point& a) const;
   Point operator^(const Point& a) const;
   /**
    * @brief Returns the third coordinate of the vector resulting from
    * the cross product.
    */
   double operator|(const Point& a) const;
   double operator[](size_t index) const
   {
      return _coordinates.at(index);
   }
   double& operator[](size_t index) { return _coordinates.at(index); }
   double operator[](const std::string& coordinate_name) const;
   double& operator[](const std::string& coordinate_name);

   size_t size() const { return _coordinates.size(); }
   size_t dimension() const;
   double length() const { return distance(Point(), *this); }

   void resize(size_t size);
   void zeroing();
   /**
    * @brief Convert Cartesian coordinates to polar coordinates of
    * `this` point
    *
    * @param o origin (axes intersection point)
    * @return void (None)
    */
   void toPolarCoord2(const Point& o);
   /**
    * @brief Convert `this` point Cartesian coordinates to polar
    * coordinates Point(0,0) - origin
    * @return void (None)
    */
   void toPolarCoord2();
   static double distance(const Point& a, const Point& b);
   double distance(const Point& other) const;
   static size_t min_size(const Point* arr, int n);
   static size_t max_size(const Point* arr, int n);
   static size_t max_size(const std::vector<Point>& arr);
   static Point middle(const Point* arr, int n);
   static Point middle(const Point& a, const Point& b);
   static Point middle(const std::vector<Point>& array);

   static double cos(const Point& a, const Point& b);
   static double cos(const Point& a, const Point& b, const Point& o);
   static double angle(const Point& a, const Point& b);
   static double angle(const Point& a, const Point& b,
                       const Point& o);

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
    * @return Angle Result angle (in degrees)
    */
   static Angle angleDegrees(const Point& a, const Point& o,
                             const Point& b);
   /**
    * @brief Calculates the angle between the positive direction of
    * the OX axis and b
    *
    * @return double Angle (from 0 to 2*pi)
    */
   static double angle360(const Point& b);
   /**
    * @brief Calculates the angle between the positive direction of
    * the OX axis and `this` point
    *
    * @return double Angle (from 0 to 2*pi)
    */
   double angle360() const;

   /**
    * @brief Calculates the angle between the positive direction of
    * the OX axis and b - o
    *
    * @return double Angle (from 0 to 2*pi)
    */
   static double angle360(const Point& b, const Point& o);
   /**
    * @brief Calculates the angle between a - o and b - o
    *
    * @return double Angle (from 0 to 2*pi)
    */
   static double angle360(const Point& a, const Point& b,
                          const Point& o);
   /**
    * @brief Does check is point p lies inside the angle p1p2p3
    *
    * @return bool
    */
   static bool isInsideAngle(const Point& p1, const Point& p2,
                             const Point& p3, const Point& p);
   /**
    * @brief Does check is point has one or both coordinates at
    * infinity
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
    * @param size size of result Point. Example: with size=2 Point
    * will be 2D.
    * @return Point
    */
   static Point getRandom(int min, int max, size_t size = 2);
   std::string to_string();
};

std::ostream& operator<<(std::ostream& out, const Point& p);

#endif // GEOMETRY_LIB_POINT_HPP
