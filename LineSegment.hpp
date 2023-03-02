#ifndef GEOMETRY_LIB_LINESEGMENT_HPP
#define GEOMETRY_LIB_LINESEGMENT_HPP

#include "Line.hpp"

class LineSegment
{
  private:
   Line _line;
   Point _endpoints[2];

   /**
    * @brief According to the values of vector products, it determines
    * whether points are on the same side of the segment
    */
   static bool areOnSameSide(double a, double b);

  public:
   LineSegment(const Point& a, const Point& b);
   LineSegment(const Line& l, const Point endpoints[2]);

   std::pair< Point, Point > getEndpoints() const;
   double length() const;

   /**
    * @brief Move this line segment along the specified segment
    *
    * @param other The line segment along which the segment will move. One of it
    * endpoints must be endpoint of this segment
    */
   LineSegment move(const LineSegment& other) const;
   
   /**
    * @brief Does check is point belongs to this line
    *
    * @param point source point
    * @return true if belongs
    * @return false if not
    */
   // bool isBelongs(Point point) const;

   ~LineSegment();

   /**
    * @brief Checks if the segments p1p2 and p3p4 intersect
    * 
    */
   bool static isIntersection(const Point& p1, const Point& p2, const Point& p3, const Point& p4);

   bool isIntersection(const LineSegment& ls) const;
};

#endif // GEOMETRY_LIB_LINESEGMENT_HPP
