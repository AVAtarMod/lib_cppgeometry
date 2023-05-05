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
std::pair<int, int> Graph::binSearch(const T1& t1, const std::vector<T2>& t2,
                                     bool (*op)(const T1&, const T2&))
{
    std::pair<int, int> ans { 0, t2.size() - 1 };
    int mid = (ans.second - ans.first) / 2;
    while (ans.first < ans.second - 1) {
        if (op(t1, t2[mid]))
           mid = ans.first;
        else
           mid = ans.second;
    }
    if (ans.second == ans.first)
        if (ans.first == 0)
           ans.second = 1;
        else if (ans.first == t2.size() - 1)
           ans.first = t2.size() - 1;
    return ans;
}

bool Graph::isLower(const Point& p, const NumberedPoint& vert)
{
    return !isZero(vert._p["y"] - p["y"]) && vert._p["y"] < p["y"];
}

bool Graph::isOnLeft(const Point& p, const LineSegment& edge)
{
    Line l = edge.getLine();
    switch (l.getType()) {
        case LineType::CONST_X:
           return !isZero(l.B() - p["x"]) && l.B() < p["x"];
        case LineType::NORMAL:
           double y = l.y(p["x"]);
           if (l.K() < 0)
              return !isZero(y - p["y"]) && y > p["y"];
           else
              return !isZero(y - p["y"]) && y < p["y"];
        default:
           return true;
    }
}

Polygon Graph::localizationOfAPoint(const Point& p) const
{
    std::vector<NumberedPoint> points(_points);
    std::sort(
      points.begin(), points.end(), [](NumberedPoint a, NumberedPoint b) {
         return !isZero(a._p["y"] - b._p["y"]) && a._p["y"] < b._p["y"];
      });
    std::pair<int, int> vertical_range =
      binSearch(p, points, isLower); // need to make a pointer

    // Finding edges that intersect the found area
    std::vector<LineSegment> edges;
    int i, j;
    Line l,
      l_horizontal[2] { Line(points[vertical_range.first]._p,
                             points[vertical_range.first]._p + Point(1, 0)),
                        Line(points[vertical_range.second]._p,
                             points[vertical_range.second]._p + Point(1, 0)) };
    Point endpoints[2];
    for (i = 0; i <= vertical_range.first; i++)
        for (j = vertical_range.second; j < points.size(); j++)
           if (_adjacencyMatrix[points[i]._numder][points[j]._numder] == 1) {
              l = Line(points[i]._p, points[j]._p);
              endpoints[0] = Line::intersect(l, l_horizontal[0]);
              endpoints[1] = Line::intersect(l, l_horizontal[1]);
              edges.push_back(LineSegment(l, endpoints));
           }

    std::sort(edges.begin(), edges.end(), [](LineSegment a, LineSegment b) {
       return a.getBegin()["x"] < b.getBegin()["x"] ||
              a.getEnd()["x"] < b.getEnd()["x"];
    });
    std::pair<int, int> line_range = binSearch(p, edges, isOnLeft); // need to make a pointer

    std::vector<Point> polygon_points;
    polygon_points.push_back(edges[line_range.first].getBegin());
    if (edges[line_range.second].getBegin() != polygon_points[0])
        polygon_points.push_back(edges[line_range.second].getBegin());
    polygon_points.push_back(edges[line_range.second].getEnd());
    if (edges[line_range.second].getBegin() !=
        polygon_points[polygon_points.size() - 1])
        polygon_points.push_back(edges[line_range.second].getBegin());
    return Polygon(polygon_points);
}

Graph::~Graph()
{
    _adjacencyMatrix.~vector();
    _points.~vector();
}