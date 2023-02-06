#include "Line.hpp"
#include "functions.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

class Line::LineEquation
{
  private:
   Point _pointA, _pointB;
   double _k, _b, _x, _y;
   double xDiff, yDiff;
   bool _inited = false;
   LineType type;

   void initByLineType();

  public:
   LineEquation() {};
   LineEquation(const Point& a, const Point& b);
   LineEquation(const ComplexNumber& a, const ComplexNumber& b);

   double K() const { return _k; }
   double B() const { return _b; }

   double xConst() const { return _x; }
   double yConst() const { return _y; }

   bool isInited() const { return _inited; }

   LineType getType() const { return type; }
};

Point intersectEqualType(const Line& first, const Line& second);
Point intersectSubNormalType(const Line& first, const Line& second);

#pragma region Constructors
void Line::finishInit(const LineEquation& initedEquation)
{
   if (!initedEquation.isInited())
      throw std::runtime_error(
        "Cannot finish line initialization with not inited equation!");

   _k = initedEquation.K();
   _b = initedEquation.B();
   _type = initedEquation.getType();
   switch (_type) {
      case LineType::CONST_X:
         _x = initedEquation.xConst();
         break;
      case LineType::CONST_Y:
         _y = initedEquation.yConst();
         break;
      default:
         break;
   }
}

Line::Line(double k, double b) : _k(k), _b(b)
{
   if (std::isinf(k)) {
      throw std::runtime_error(
        "Cannot construct line x = ? from equation y = kx + b");
   } else if (k == 0) {
      _type = LineType::CONST_Y;
      _y = b;
   }
}

Line::Line(const std::pair< Point, Point >& pair)
{
   LineEquation equation = LineEquation(pair.first, pair.second);
   finishInit(equation);
}

Line::Line(const ComplexNumber& first, const ComplexNumber& second)
{
   LineEquation equation = LineEquation(first, second);
   finishInit(equation);
}

Line::Line(const Line& source)
{
   *this = source;
}
#pragma endregion
#pragma region Getters and methods
double Line::y(double x) const
{
   switch (_type) {
      case LineType::CONST_X:
         return (_x == x) ? std::numeric_limits< double >::infinity() : 0;
      case LineType::CONST_Y:
         return _y;
      case LineType::NORMAL:
         return _k * x + _b;
      default:
         return 0;
   }
}

double Line::x(double y) const
{
   switch (_type) {
      case LineType::CONST_X:
         return _x;
      case LineType::CONST_Y:
         return (_y == y) ? std::numeric_limits< double >::infinity() : 0;
      case LineType::NORMAL:
         return (y - _b) / _k;
      default:
         return 0;
   }
}

bool Line::isInX(double x) const
{
   switch (_type) {
      case LineType::CONST_X:
         return _x == x;
      case LineType::CONST_Y:
      case LineType::NORMAL:
         return true;
      default:
         return false;
   }
}

bool Line::isInY(double y) const
{
   switch (_type) {
      case LineType::CONST_X:
      case LineType::CONST_Y:
         return _y == y;
      case LineType::NORMAL:
         return true;
      default:
         return false;
   }
}

bool Line::isBelongs(Point point) const
{
   switch (_type) {
      case LineType::CONST_X:
         return _x == point.X();
      case LineType::CONST_Y:
         return _y == point.Y();
      case LineType::NORMAL:
         return almost_equal(y(point.X()), point.Y(), 2);
      default:
         return false;
   }
}
#pragma endregion

void Line::operator=(const Line& other)
{
   _k = other._k;
   _b = other._b;
   _type = other._type;
}

Line Line::makePerpendicular(const Line& to, const Point& from)
{
   /**
    * @brief Random constant, should be > 0
    */
   const unsigned int yDiff = 5;

   switch (to.getType()) {
      case LineType::CONST_X:
         // Perpendicular is CONST_Y, y = from.Y
         return Line { 0, from.Y() };
      case LineType::CONST_Y:
         // Perpendicular is CONST_X, x = from.X
         // We should use constructor by 2 points because y = kx + b not
         // represent equation x = const
         return Line { from, Point { from.X(), from.Y() + yDiff } };
      case LineType::NORMAL:
         // Perpendicular is NORMAL, k = -1/to.k and 'to' on perpendicular
         {
            const double k = -1 / to.K();
            const double b = from.Y() + from.X() / to.K();
            return Line(k, b);
         }
      default:
         return Line(0, 0);
   }
}

Point Line::intersect(const Line& first, const Line& second)
{

   if (first._type == second._type) {
      return intersectEqualType(first, second);
   } else if (first._type == LineType::NORMAL ||
              second._type == LineType::NORMAL) {
      return intersectSubNormalType(first, second);
   } else {
      // first is CONST_Y, second is CONST_X or vice versa
      // Make first is CONST_X
      Line f { first }, s { second };
      if (f._type != LineType::CONST_X)
         Line::swap(f, s);

      return Point { f._x, s._y };
   }
}

bool Line::isPerpendicular(const Line& other, double precision) const
{
   switch (_type) {
      case LineType::CONST_X:
         return other._type == LineType::CONST_Y;
      case LineType::CONST_Y:
         return other._type == LineType::CONST_X;
      case LineType::NORMAL: {
         return areEqual(other._k, -1 / _k, precision);
      }
      default:
         break;
   }
   return false;
}

bool Line::isOnSameLine(const Point& a, const Point& b, const Point& c)
{
   return Line(a, b).isBelongs(c);
}
bool Line::isOnSameLine(const ComplexNumber& a, const ComplexNumber& b,
                        const ComplexNumber& c)
{
   return isOnSameLine(
     static_cast< Point >(a), static_cast< Point >(b), static_cast< Point >(c));
}

void Line::swap(Line& left, Line& right)
{
   Line tmp { left };
   left = right;
   right = tmp;
}

#pragma region Implementation
void Line::LineEquation::initByLineType()
{
   switch (type) {
      case LineType::CONST_X:
         // Y may be any, x = const
         _k = 0;
         _b = std::numeric_limits< double >::infinity();
         _x = _pointB.X();
         break;
      case LineType::CONST_Y:
         // X may be any, y = const
         _k = 0;
         _b = 0;
         _y = _pointB.Y();
         break;
      case LineType::NORMAL:
         // Normal line, y = kx + b
         _k = yDiff / xDiff;
         _b = (-_pointA.X() * yDiff + _pointA.Y() * xDiff) / xDiff;
         break;
      default:
         break;
   }
}

Line::LineEquation::LineEquation(const ComplexNumber& a,
                                 const ComplexNumber& b) :
  LineEquation(static_cast< Point >(a), static_cast< Point >(b))
{
}

Line::LineEquation::LineEquation(const Point& a, const Point& b)
{
   if (a == b || std::isinf(a.X()) || std::isinf(b.X()) || std::isinf(a.Y()) ||
       std::isinf(b.Y()))
      throw std::runtime_error(
        "Cannot create line from 2 equal points, or coordinates incorrect (a.e. Inf)");

   _pointA = a, _pointB = b;

   yDiff = _pointB.Y() - _pointA.Y();
   xDiff = _pointB.X() - _pointA.X();
   type = (xDiff == 0) ? LineType::CONST_X
                       : ((yDiff == 0) ? LineType::CONST_Y : LineType::NORMAL);

   initByLineType();
   _inited = true;
   return;
}

Point intersectEqualType(const Line& first, const Line& second)
{
   switch (first.getType()) {
      case LineType::CONST_X:
      case LineType::CONST_Y:
         return Point { std::numeric_limits< double >::infinity(),
                        std::numeric_limits< double >::infinity() };
      case LineType::NORMAL: {
         const double x = (second.B() - first.B()) / (first.K() - second.K());
         const double y = second.y(x);
         return Point { x, y };
      }
      default:
         return Point();
   }
}

Point intersectSubNormalType(const Line& first, const Line& second)
{
   Line f { first }, s { second };
   /**
    * @brief Random constant value
    */
   const unsigned int cv = 0;

   // Make first is NORMAL
   if (f.getType() != LineType::NORMAL)
      Line::swap(f, s);

   switch (s.getType()) {
      case LineType::CONST_X:
         return Point { s.x(cv), f.y(s.x(cv)) };
      case LineType::CONST_Y:
         return Point { f.x(s.y(cv)), s.y(cv) };
      default:
         return Point();
   }
}
#pragma endregion
