#ifndef COURSEWORK_4_1_CIRCLEARC_HPP
#define COURSEWORK_4_1_CIRCLEARC_HPP

#include "Angle.hpp"
#include "Circle.hpp"

class CircleArc
{
  private:
   const Circle& _circle;
   /**
    * @brief Describes boundaries info:
    * 0 - left angle
    * 1 - right angle
    */
   Angle _boundaries[2];

   /**
    * @brief Does finish initialization by 3 points. The _circle must be defined
    * before call.
    */
   void initByExactValues(const Point& a, const Point& b,
                          const Point& betweenAB);

  public:
   /**
    * @brief Construct a new circle arc by source circle and 3 points. All
    * points should to be at the circle and not equal each other.
    *
    * @param circle Source circle
    * @param a Begin point of the arc
    * @param b End point of the arc
    * @param betweenAB A point between a,b. Needed for unambiguous
    * constructing resulted arc.
    */
   CircleArc(const Circle& circle, const Point& a, const Point& b,
             const Point& betweenAB);
   /**
    * @param approximate Construct circle arc by approximate points. Exact
    * values will be given from the circle.
    */
   CircleArc(const Circle& circle, const Point& a, const Point& b,
             const Point& betweenAB, bool approximate);

   /**
    * @brief Does check is Point belongs to this circle arc
    *
    * @param point A point that need to check
    */
   bool isBelongs(const Point& point) const;
   Point middle() const;

   ~CircleArc();
};

#endif // COURSEWORK_4_1_CIRCLEARC_HPP
