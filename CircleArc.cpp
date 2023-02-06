#include "CircleArc.hpp"

#include "functions.hpp"
#include <cmath>
#include <stdexcept>

void CircleArc::initByExactValues(const Point& a, const Point& b,
                                  const Point& betweenAB)
{
   bool isValidInput = _circle.isBelongs(a) && _circle.isBelongs(b) &&
                       _circle.isBelongs(betweenAB) && a != b && b != betweenAB;
   if (!isValidInput)
      throw std::invalid_argument(
        "CircleArc: Cannot construct. One or more points not at the circle or points equal");

   Angle angles[3] { _circle.getAngle(a),
                     _circle.getAngle(b),
                     _circle.getAngle(betweenAB) };

   const Angle& min = std::min(angles[0], angles[1]);
   const Angle& max = (min == angles[0]) ? angles[1] : angles[0];
   const Angle& between = angles[2];

   if (between > max) {
      _boundaries[0] = min;
      _boundaries[1] = max;
   } else {
      _boundaries[0] = max;
      _boundaries[1] = min;
   }
}

CircleArc::CircleArc(const Circle& circle, const Point& a, const Point& b,
                     const Point& betweenAB) :
  _circle(circle)
{
   initByExactValues(a, b, betweenAB);
}

CircleArc::CircleArc(const Circle& circle, const Point& a, const Point& b,
                     const Point& betweenAB, bool approximate) :
  _circle(circle)
{
   if (approximate) {
      initByExactValues(_circle.getExactPoint(a),
                        _circle.getExactPoint(b),
                        _circle.getExactPoint(betweenAB));
   } else
      initByExactValues(a, b, betweenAB);
}

bool CircleArc::isBelongs(const Point& point) const
{
   Point tmp = point;
   if (!_circle.isBelongs(tmp))
      tmp = _circle.getExactPoint(tmp);

   const Angle &left = _boundaries[0], &right = _boundaries[1];
   const Angle& input = _circle.getAngle(point);

   if (left <= input && input <= right)
      return true;
   else
      return false;

   throw std::runtime_error(
     "CircleArc::isBelongs: passed invalid point, we tried to fix it but no success.");
}

Point CircleArc::middle() const
{
   const Angle &left = _boundaries[0], &right = _boundaries[1];
   double degrees;
   if (left < right) {
      degrees = (left + right).degrees() / 2;
      return _circle.getPoint(Angle(degrees));
   } else if (left > right) {
      degrees = (left + (Angle::fullAngle() - left + right) / 2).degrees();
      return _circle.getPoint(degrees);
   } else {
      return _circle.getPoint(left);
   }

   throw std::runtime_error(
     "CircleArc::middle: cannot construct middle. This is bug.");
}

CircleArc::~CircleArc()
{
   _boundaries[0].~Angle();
   _boundaries[1].~Angle();
}
