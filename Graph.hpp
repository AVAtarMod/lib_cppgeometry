#ifndef GRAPH_LIB
#define GRAPH_LIB

//! The quality of the code in this library is so low (it's very complex and hard to read)

#include <vector>
#include <iostream>
#include "Point.hpp"
#include "Polygon.hpp"
#include "functions.hpp"

using matrix_t = std::vector<std::vector<int>>;

class Graph
{
  private:
   struct NumberedPoint;

   matrix_t _adjacencyMatrix;
   std::vector<NumberedPoint> _points;

   template<typename T1, typename T2>
   static std::pair<int, int> binSearch(const T1& t1, const std::vector<T2>& t2,
                                        bool (*op)(const T1&, const T2&));
   static bool isLower(const Point& p, const NumberedPoint& vert);
   static bool isOnLeft(const Point& p, const LineSegment& edge);

  public:
   enum Method
   {
      BY_DEPTH,
      BY_WIDTH
   };

   // Graph();
   Graph(const matrix_t& adjacencyMatrix, std::vector<Point> points);
   //  Graph(const List1D<List1D<int>> adjacencyList);

   void operator=(const Graph& graph);

   //  List1D<List1D<int>> getAdjacencyList() const;
   const matrix_t getAdjacencyMatrix() const;
   Graph getSpanningTree(Method m) const;
   void print() const;
   bool validate() const;

   friend std::istream& operator>>(std::istream& input, Graph& number);

   Polygon localizationOfAPoint(const Point& p) const;

   ~Graph();
};

std::istream& operator>>(std::istream& input, Graph& number);

#endif // GRAPH_LIB
