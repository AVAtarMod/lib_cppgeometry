#ifndef GEOMETRY_LIB_COMPLEXNUMBER_HPP
#define GEOMETRY_LIB_COMPLEXNUMBER_HPP

#include "Point.hpp"
#include <iostream>

// TODO write comments

class ComplexNumber
{
  private:
   double _imaginary;
   double _real;
   friend std::ostream& operator<<(std::ostream& out,
                                   const ComplexNumber& number);
   friend std::istream& operator>>(std::istream& in, ComplexNumber& number);

  public:
   ComplexNumber(double real = 0, double imaginary = 0);
   ComplexNumber(const ComplexNumber& source);
   ComplexNumber(const Point& point);

   void operator=(const ComplexNumber& b);

   ComplexNumber operator+(const ComplexNumber& b) const;
   ComplexNumber operator-(const ComplexNumber& b) const;
   ComplexNumber operator*(const ComplexNumber& b) const;
   // ComplexNumber operator/(const ComplexNumber& b) const;

   explicit operator Point() const { return Point(Re(), Im()); }
   Point toPoint() const { return static_cast< Point >(*this); }

   bool operator==(const ComplexNumber& b) const;
   bool operator!=(const ComplexNumber& b) const;

   static ComplexNumber conjugate(const ComplexNumber& number);
   /**
    * @brief Round ComplexNumber to specified digits after decimal separator
    *
    * @param number source number
    * @param ulp amount digits after decimal separator
    * @return ComplexNumber - Rounded number
    */
   static ComplexNumber round(const ComplexNumber& number, int8_t ulp);

   const double& Re() const { return _real; }
   const double& Im() const { return _imaginary; }
   const double Arg() const { return atan2(_imaginary, _real); }
   const double Mod() const { return sqrt(Mod2()); }
   const double Mod2() const { return _real * _real + _imaginary * _imaginary; }

   static ComplexNumber zero() { return ComplexNumber(0, 0); }
};

#endif // GEOMETRY_LIB_COMPLEXNUMBER_HPP
