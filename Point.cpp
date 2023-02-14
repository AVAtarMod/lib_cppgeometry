#include "Point.hpp"

#include <cmath>

Point::Point(double x, double y){
   _coord = new double[2];
   _coord[0] = x;
   _coord[1] = y;
}

bool Point::isAtInfinity(const Point& point)
{
	for (int i = 0; i < point.Size(); i++)
		if (std::isinf(point[i])) return true;
	return false;
}
