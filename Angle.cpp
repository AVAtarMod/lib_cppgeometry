#include "Angle.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

struct AngleLimits
{
   double min;
   double max;
};
AngleLimits getLimit(Angle::Type type)
{
   switch (type) {
      case Angle::Type::Any:
         return { -360, 360 };
      case Angle::Type::Positive:
         return { 0, 360 };
      default:
         break;
   }
}

Angle::Angle(double value, Type type)
{
   auto limits = getLimit(type);
   if (limits.min <= value && value <= limits.max && !std::isinf(value))
      _degrees = value;
   else
      throw std::runtime_error(
        "Cannot construct Angle by incorrect degrees value (" +
        std::to_string(value) + ")");
}
Angle::Angle(double min, double value, double max)
{
   if (min <= value && value <= max && !std::isinf(value))
      _degrees = value;
   else
      throw std::runtime_error(
        "Cannot construct Angle by incorrect degrees value (" +
        std::to_string(value) + ")");
}

bool Angle::operator==(const Angle& other) const
{
   return _degrees == other._degrees;
}
bool Angle::operator!=(const Angle& other) const
{
   return !(*this == other);
}
bool Angle::operator>(const Angle& other) const
{
   return _degrees > other._degrees;
}
bool Angle::operator<(const Angle& other) const
{
   return _degrees < other._degrees;
}
bool Angle::operator<=(const Angle& other) const
{
   return *this == other || *this < other;
}
bool Angle::operator>=(const Angle& other) const
{
   return *this == other || *this > other;
}

Angle Angle::operator+(const Angle& other) const
{
   double result = _degrees + other._degrees;
   if (result > 360.0) {
      result = std::fmod(result, fullAngle()._degrees);
   }
   return Angle(result);
}

Angle Angle::operator-(const Angle& other) const
{
   return Angle(_degrees - other._degrees);
}
Angle Angle::operator/(const double& number) const
{
   return Angle(_degrees / number);
}

Angle Angle::fullAngle()
{
   return Angle(360.0);
}
