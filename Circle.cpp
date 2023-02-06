#include "Circle.hpp"

#include "Line.hpp"
#include "functions.hpp"
#include <cmath>
#include <exception>

Circle::Circle(const Point& center, double radius)
{
   bool isValidInput = !std::isinf(radius) && !Point::isAtInfinity(center);
   if (!isValidInput)
      throw std::runtime_error(
        "Cannot construct the circle for infinity radius or center");

   _center = center;
   _radius = radius;
}
Circle::Circle(const Point& a, const Point& b, const Point& c)
{
   if (Line(a, b).isBelongs(c))
      throw std::runtime_error(
        "Cannot construct the circle by 3 points. They belongs same line");

   /**
    * @brief Calculted coordinates of circle _center
    * @see https://en.wikipedia.org/wiki/Ellipse#Circles
    */
   double X, Y;
   const double &aX = a.X(), &aY = a.Y(), &bX = b.X(), &bY = b.Y(), &cX = c.X(),
                &cY = c.Y();
   const double denominator =
     (aX * (bY - cY) + bX * (cY - aY) + cX * (aY - bY));

   X = (-1.0 / 2.0) *
       (aY * ((power(bX, 2) + power(bY, 2)) - (power(cX, 2) + power(cY, 2))) +
        bY * ((power(cX, 2) + power(cY, 2)) - (power(aX, 2) + power(aY, 2))) +
        cY * ((power(aX, 2) + power(aY, 2)) - (power(bX, 2) + power(bY, 2)))) /
       denominator;
   Y = (1.0 / 2.0) *
       (aX * ((power(bX, 2) + power(bY, 2)) - (power(cX, 2) + power(cY, 2))) +
        bX * ((power(cX, 2) + power(cY, 2)) - (power(aX, 2) + power(aY, 2))) +
        cX * ((power(aX, 2) + power(aY, 2)) - (power(bX, 2) + power(bY, 2)))) /
       denominator;

   _center = Point(X, Y);
   _radius = sqrt(power(aX - _center.X(), 2) + power(aY - _center.Y(), 2));
}

bool Circle::isBelongs(const Point& a) const
{
   return almost_equal(power(a.X() - _center.X(), 2) +
                         power(a.Y() - _center.Y(), 2),
                       power(_radius, 2));
}

bool Circle::isBelongs(const Point& a, int8_t dds) const
{
   if (dds < 0)
      throw std::invalid_argument(
        "Circle::isBelongs: cannot set negative precision");

   return round(power(a.X() - _center.X(), 2) + power(a.Y() - _center.Y(), 2),
                dds) == round(power(_radius, 2), dds);
}

std::pair< double, double > Circle::y(double x) const
{
   const double value =
     std::sqrt(power(_radius, 2) - power(x - _center.X(), 2));
   return { _center.Y() + value, _center.Y() - value };
}
std::pair< double, double > Circle::x(double y) const
{
   const double value =
     std::sqrt(power(_radius, 2) - power(y - _center.Y(), 2));
   return { _center.X() + value, _center.X() - value };
}
Angle Circle::getAngle(const Point& a) const
{
   Point exact { a };
   if (!this->isBelongs(exact))
      exact = getExactPoint(exact);
   Line l(_center, exact);
   switch (l.getType()) {
      case LineType::CONST_X:
         return (a.Y() > _center.Y()) ? Angle(90.0) : Angle(270.0);
      case LineType::CONST_Y:
         return (a.X() > _center.X()) ? Angle(0.0) : Angle(180.0);
      case LineType::NORMAL: {
         double degree = std::atan(l.K()) * 180 / M_PI;
         if (a.X() < _center.X())
            degree = 360.0 - degree;
         if (degree < 0)
            degree = 360.0 + degree;
         return Angle(degree);
      }
      default:
         break;
   }
   throw std::runtime_error("Cannot get angle of the point on circle");
}
Point Circle::getPoint(const Angle& a) const
{
   /**
    * @brief Convert degrees in radians and getting not scaled X
    */
   const double xRaw = std::cos(a.degrees() * M_PI / 180.0);
   const double x = xRaw * _radius + _center.X();
   auto yValues = y(x);
   const double y = (a.degrees() > 180.0)
                      ? std::min(yValues.first, yValues.second)
                      : std::max(yValues.first, yValues.second);
   return Point(x, y);
}

Point Circle::getExactPoint(const Point& a, ApproximationMethod m) const
{
   switch (m) {
      case BY_X_AXIS: {
         auto yValues = y(a.X());
         const double y = (std::fabs(yValues.first - a.X()) <
                           std::fabs(yValues.second - a.X()))
                            ? yValues.first
                            : yValues.second;
         return Point(a.X(), y);
      }
      case BY_Y_AXIS: {
         auto xValues = x(a.Y());
         const double x = (std::fabs(xValues.first - a.X()) <
                           std::fabs(xValues.second - a.X()))
                            ? xValues.first
                            : xValues.second;
         return Point(x, a.Y());
      }
      default:
         break;
   }
   return Point::zero();
}

Circle::~Circle()
{
   _center.~Point();
}
