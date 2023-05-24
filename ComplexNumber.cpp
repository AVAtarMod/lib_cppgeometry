#include "ComplexNumber.hpp"
#include "Line.hpp"

#include "functions.hpp"

static const ComplexNumber zero = ComplexNumber(0, 0);

ComplexNumber::ComplexNumber(const Point& point)
{
   _real = point["x"];
   _imaginary = point["y"];
}

ComplexNumber::ComplexNumber(double real, double imaginary) :
  _imaginary(imaginary), _real(real)
{
}

ComplexNumber::ComplexNumber(const ComplexNumber& source) :
  _imaginary(source.Im()), _real(source.Re())
{
}

std::ostream& operator<<(std::ostream& out, const ComplexNumber& number)
{
   out << number._real << " + " << number._imaginary << "i";
   return out;
}

std::istream& operator>>(std::istream& in, ComplexNumber& number)
{
   in >> number._real >> number._imaginary;
   return in;
}

void ComplexNumber::operator=(const ComplexNumber& b)
{
   this->_real = b.Re();
   this->_imaginary = b.Im();
   return;
}

ComplexNumber ComplexNumber::operator+(const ComplexNumber& b) const
{
   return ComplexNumber(this->Re() + b.Re(), this->Im() + b.Im());
}
ComplexNumber ComplexNumber::operator-(const ComplexNumber& b) const
{
   return ComplexNumber(this->Re() - b.Re(), this->Im() - b.Im());
}
ComplexNumber ComplexNumber::operator*(const ComplexNumber& b) const
{
   return ComplexNumber(this->Re() * b.Re() - this->Im() * b.Im(),
                        this->Re() * b.Im() + this->Im() * b.Re());
}
ComplexNumber ComplexNumber::operator/(const ComplexNumber& b) const
{
   double mod2 = b.Mod2();
   return ComplexNumber(
     (this->Re() * b.Re() + this->Im() * b.Im()) / mod2,
     (this->Im() * b.Re() - this->Re() * b.Im()) / mod2);
}

bool ComplexNumber::operator==(const ComplexNumber& b) const
{
   return this->Re() == b.Re() && this->Im() == b.Im();
}
bool ComplexNumber::operator!=(const ComplexNumber& b) const
{
   return !(*this == b);
}

ComplexNumber ComplexNumber::conjugate(const ComplexNumber& number)
{
   return ComplexNumber(number.Re(), -number.Im());
}

ComplexNumber ComplexNumber::round(const ComplexNumber& number, int8_t ulp)
{
   return ComplexNumber(::round(number._real, ulp),
                        ::round(number._imaginary, 2));
}
