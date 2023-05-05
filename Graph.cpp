#include "Graph.hpp"

struct Graph::NumberedPoint {
    unsigned int _numder;
    Point _p;

    NumberedPoint()
    {
       _numder = 0;
       _p = Point();
    }

    NumberedPoint(Point p, unsigned int numder) {
        _numder = numder;
        _p = Point(p);
    }
};

Graph::Graph(const matrix_t& adjacencyMatrix, std::vector<Point> points)
{
    _adjacencyMatrix = matrix_t(adjacencyMatrix);
    _points = std::vector<NumberedPoint>(points.size());
    for (int i = 0; i < _points.size(); i++)
        _points[i] = NumberedPoint(points[i], i);
}

template<typename T1, typename T2>
std::unique_ptr<std::pair<int, int>> Graph::binSearch(
  const T1& t1, const std::vector<T2>& t2,
  bool (*less)(const T1&, const T2&))
{
    std::unique_ptr<std::pair<int, int>> ans;
    if (!less(t1, t2[0]) && less(t1, t2[t2.size() - 1])) { // And if t1 == t2[t2.size() - 1]?! This is problem!
        ans = std::make_unique<std::pair<int, int>>(0, t2.size() - 1);
        int mid;
        while (ans->first < ans->second - 1) {
           mid = (ans->second + ans->first) / 2;
           if (less(t1, t2[mid]))
              ans->second = mid;
           else
              ans->first = mid;
        }
    }
    return ans;
}

bool Graph::isLower(const Point& p, const NumberedPoint& vert)
{
    return !isZero(vert._p["y"] - p["y"]) && p["y"] < vert._p["y"];
}

bool Graph::isOnLeft(const Point& p, const LineSegment& edge)
{
    Line l = edge.getLine();
    switch (l.getType()) {
        case LineType::CONST_X:
           return !isZero(l.B() - p["x"]) && p["x"] < l.B();
        case LineType::NORMAL: {
           double y = l.y(p["x"]);
           if (l.K() < 0)
              return !isZero(y - p["y"]) && p["y"] < y;
           else
              return !isZero(y - p["y"]) && p["y"] > y;
        }
        default:
           return true;
    }
}

std::unique_ptr<Polygon> Graph::localizationOfAPoint(
  const Point& p) const
{
    std::unique_ptr<Polygon> ans;
    std::vector<NumberedPoint> points(_points);
    std::sort(points.begin(),
              points.end(),
              [](NumberedPoint a, NumberedPoint b) {
                 return !isZero(a._p["y"] - b._p["y"]) &&
                        a._p["y"] < b._p["y"];
              });
    auto horizontal_range =
      binSearch(p, points, isLower);

    // Finding edges that intersect the found area
    if (horizontal_range) {
        std::vector<LineSegment> edges;
        int i, j;
        Line l, l_horizontal[2] {
           Line(points[horizontal_range->first]._p,
                points[horizontal_range->first]._p + Point(1, 0)),
           Line(points[horizontal_range->second]._p,
                points[horizontal_range->second]._p + Point(1, 0))
        };
        Point endpoints[2];
        for (i = 0; i <= horizontal_range->first; i++)
           for (j = horizontal_range->second; j < points.size(); j++)
              if (_adjacencyMatrix[points[i]._numder]
                                  [points[j]._numder] == 1) {
                 l = Line(points[i]._p, points[j]._p);
                 endpoints[0] = Line::intersect(l, l_horizontal[0]);
                 endpoints[1] = Line::intersect(l, l_horizontal[1]);
                 edges.push_back(LineSegment(l, endpoints));
              }

        std::sort(edges.begin(),
                  edges.end(),
                  [](LineSegment a, LineSegment b) {
                     return a.getBegin()["x"] < b.getBegin()["x"] ||
                            a.getEnd()["x"] < b.getEnd()["x"];
                  });
        auto line_range = binSearch(p, edges, isOnLeft);

        if (line_range) {
           std::vector<Point> polygon_points;
           polygon_points.push_back(
             edges[line_range->first].getBegin());
           if (edges[line_range->second].getBegin() !=
               polygon_points[0])
              polygon_points.push_back(
                edges[line_range->second].getBegin());
           polygon_points.push_back(
             edges[line_range->second].getEnd());
           if (edges[line_range->second].getBegin() !=
               polygon_points[polygon_points.size() - 1])
              polygon_points.push_back(
                edges[line_range->second].getBegin());
           ans = std::make_unique<Polygon>(polygon_points);
        }
    }
    return ans;
}

Graph::~Graph()
{
    _adjacencyMatrix.~vector();
    _points.~vector();
}