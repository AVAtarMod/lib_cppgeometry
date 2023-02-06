#ifndef COURSEWORK_4_1_CIRCLE_HPP
#define COURSEWORK_4_1_CIRCLE_HPP

#include "Angle.hpp"
#include "Point.hpp"
#include <cstdint>
#include <utility>

class Circle
{
  private:
   Point _center;
   double _radius;

  public:
   enum ApproximationMethod
   {
      BY_X_AXIS, // Point's X coordinate should be remain untouched
      BY_Y_AXIS  // Point's Y coordinate should be remain untouched
   };

   Circle(const Point& center, double radius);
   Circle(const Point& a, const Point& b, const Point& c);

   Point center() const { return _center; }
   const double& radius() const { return _radius; }

   std::pair< double, double > y(double x) const;
   std::pair< double, double > x(double y) const;

   bool isBelongs(const Point& a) const;
   /**
    * @brief Does check is Point belongs to this circle with precision up to dds
    * digits after decimal separator
    *
    * @param a A point that need to check
    * @param dds Compare precision - digits after decimal separator
    */
   bool isBelongs(const Point& a, int8_t dds) const;
   /**
    * @brief Get the angle of the point.
    *
    * @param a Point. Should belongs to this circle, otherwise angle will be
    * calculated approximately
    * @return Angle of the point
    */
   Angle getAngle(const Point& a) const;
   /**
    * @brief Get the point by its angle.
    *
    * @param a Angle
    * @return Point defined by angle
    */
   Point getPoint(const Angle& a) const;
   /**
    * @brief Get the exact point, that belongs to this circle
    *
    * @param a A point that need to clarify
    * @param m approximation method
    * @return Point - resulted point, that belongs to this circle
    */
   Point getExactPoint(const Point& a, ApproximationMethod m = BY_Y_AXIS) const;

   ~Circle();
};

#endif // COURSEWORK_4_1_CIRCLE_HPP
