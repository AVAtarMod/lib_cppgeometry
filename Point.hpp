#ifndef COURSEWORK_4_1_POINT_HPP
#define COURSEWORK_4_1_POINT_HPP

class Point
{
  private:
   double _x, _y;

  public:
   Point(double x = 0, double y = 0) : _x(x), _y(y) {};

   bool operator==(const Point& a) const
   {
      return X() == a.X() && Y() == a.Y();
   }
   bool operator!=(const Point& a) const { return !(*this == a); }
   Point operator-(const Point& other) const
   {
      return Point(_x - other._x, _y - other._y);
   }
   Point operator+(const Point& other) const
   {
      return Point(_x + other._x, _y + other._y);
   }

     const double& X() const
   {
      return _x;
   }
   const double& Y() const { return _y; }

   static Point middle(const Point& a, const Point& b)
   {
      return Point((a._x + b._x) / 2, (a._y + b._y) / 2);
   }
   static Point zero() { return Point { 0, 0 }; }
   /**
    * @brief Does check is point has one or both coordinates at infinity
    *
    * @param point A point that need to check
    */
   static bool isAtInfinity(const Point& point);
};

#endif // COURSEWORK_4_1_POINT_HPP
