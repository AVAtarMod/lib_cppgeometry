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

   /**
    * @brief Checks if the segments p1p2 and p3p4 intersect
    * 
    */
   bool static isIntersection(const Point& p1, const Point& p2, const Point& p3, const Point& p4);
   /**
    * @brief Checks if the segments this and ls intersect
    *
    */
   bool isIntersection(const LineSegment& ls) const;
   /**
    * @brief Checks if point p belongs to segment p1p2
    *
    */
   bool static isBelongs(const Point& p1, const Point& p2, const Point& p);
   /**
    * @brief Checks if point p belongs to segment this
    *
    */
   bool isBelongs(const Point& p) const;
};

#endif // GEOMETRY_LIB_LINESEGMENT_HPP
