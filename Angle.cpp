#include "Angle.hpp"

#include <cmath>
#include <stdexcept>

Angle::Angle(double value)
{
   if (0.0 <= value && value <= 360.0 && !std::isinf(value))
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
