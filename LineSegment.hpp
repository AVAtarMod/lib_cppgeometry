#ifndef GEOMETRY_LIB_LINESEGMENT_HPP
#define GEOMETRY_LIB_LINESEGMENT_HPP

#include "Line.hpp"

class LineSegment
{
  private:
   Line _line;
   Point _endpoints[2];

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
};

#endif // GEOMETRY_LIB_LINESEGMENT_HPP
