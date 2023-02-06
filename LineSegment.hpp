#ifndef COURSEWORK_4_1_LINESEGMENT_HPP
#define COURSEWORK_4_1_LINESEGMENT_HPP

#include "Line.hpp"

class LineSegment
{
  private:
   Line _line;
   Point _endpoints[2];

  public:
   LineSegment(const Point& a = Point(0, 0), const Point& b = Point(1, 0));
   LineSegment(const Line& l, const Point endpoints[2]);

   std::pair< Point, Point > getEndpoints() const;
   double length() const;

   /**
    * @brief Move this line segment along the specified segment
    *
    * @param other The line segment along which the segment will move. One of it
    * endpoints must be enpoint of this segment
    */
   LineSegment move(const LineSegment& other) const;

   ~LineSegment();
};

#endif // COURSEWORK_4_1_LINESEGMENT_HPP
