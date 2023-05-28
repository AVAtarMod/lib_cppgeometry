#include "Curve.hpp"

Curve::Curve(const std::vector<Point> points)
{
   _points = points;
}
Curve::~Curve() {}

Point multiple(const Point& p, double multiplier)
{
   Point result(p.size());
   for (size_t i = 0; i < p.size(); ++i) {
      result[i] = p[i] * multiplier;
   }
   return result;
}

Curve Curve::makeBezierCurve(const Point& P0, const Point& P1,
                             const Point& P2)
{
   const double step = 0.001;
   std::vector<Point> result(static_cast<size_t>(1 / step));
   const Point dP1 = (P1 - P0), dP2 = (P2 - P1);
   size_t index = 0;
   for (double i = 0; i < 1; i += step, ++index) {
      /**
       * @brief Q0 is on P0P1, Q1 is on P1P2
       */
      Point Q0, Q1;
      Q0 = P0 + multiple(dP1, i);
      Q1 = P1 + multiple(dP2, i);
      result[index] = Q0 + multiple(Q1 - Q0, i);
   }
   return Curve(result);
}

Curve Curve::makeBezierCurve(const Point& P0, const Point& P1,
                             const Point& P2, const Point& P3)
{
   const double step = 0.001;
   std::vector<Point> result(static_cast<size_t>(1 / step));
   const Point dP1 = (P1 - P0), dP2 = (P2 - P1), dP3 = (P3 - P2);
   size_t index = 0;
   for (double i = 0; i < 1; i += step, ++index) {
      /**
       * @brief Q0 is on P0P1, Q1 is on P1P2, Q2 is on P2P3
       */
      Point Q0, Q1, Q2;
      // Q0 represents P0 for quadratic curve
      Q0 = P0 + multiple(dP1, i);
      // Q1 represents P1 for quadratic curve
      Q1 = P1 + multiple(dP2, i);
      // Q2 represents P2 for quadratic curve
      Q2 = P2 + multiple(dP3, i);

      // Store final result in Q0, Q1
      Q0 = Q0 + multiple(Q1 - Q0, i);
      Q1 = Q1 + multiple(Q2 - Q1, i);
      result[index] = Q0 + multiple(Q1 - Q0, i);
   }
   return Curve(result);
}
