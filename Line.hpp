#ifndef COURSEWORK_4_1_LINE_HPP
#define COURSEWORK_4_1_LINE_HPP

#include "ComplexNumber.hpp"
#include "Point.hpp"
#include <tuple>

enum class LineType
{
   CONST_Y, // y = const
   CONST_X, // x = const
   NORMAL   // y = kx + b
};

/**
 * @brief Represents line by equation 'y = kx + b'
 */
class Line
{
  private:
   double _k, _b;
   /**
    * @brief Defines y or x constant value if _type is CONST_X or CONST_Y. This
    * is reference for memory optimization.
    */
   double &_x = _b, &_y = _k;
   LineType _type;

   static double getKFromPoints(const Point& a, const Point& b);
   static double getBFromPoints(const Point& a, const Point& b);

   class LineEquation;
   void finishInit(const LineEquation& initedEquation);

  public:
   Line(double k = 0, double b = 0);
   Line(const std::pair< Point, Point >& pair);
   Line(const Point& first, const Point& second) :
     Line(std::make_pair(first, second))
   {
   }
   /**
    * @brief Construct a new Line object (algorithm is same as for the two
    * Points)
    */
   Line(const ComplexNumber& first, const ComplexNumber& second);
   Line(const Line& source);

   LineType getType() const { return _type; }

   double y(double x) const;
   double x(double y) const;
   const double& K() const { return _k; }
   const double& B() const { return _b; }

   bool isInX(double x) const;
   bool isInY(double y) const;
   bool isBelongs(Point point) const;
   // bool isCollinear(const Line& other) const;
   bool isPerpendicular(const Line& other, double precision = 0.01) const;

   /**
    * @brief Swap Line @b this with @b other
    */
   void swap(Line& other) { Line::swap(*this, other); };

   void operator=(const Line& other);

   static Line makePerpendicular(const Line& to, const Point& from);
   /**
    * @brief Intersect of 2 line segments
    * @return Point, intersection point of lines, or (Inf;Inf) if lines is
    * collinear
    */
   static Point intersect(const Line& first, const Line& second);
   static bool isOnSameLine(const Point& a, const Point& b, const Point& c);
   static bool isOnSameLine(const ComplexNumber& a, const ComplexNumber& b,
                            const ComplexNumber& c);

   static void swap(Line& left, Line& right);
};

#endif // COURSEWORK_4_1_LINE_HPP
