#include "Point.hpp"

#include <cmath>

bool Point::isAtInfinity(const Point& point)
{
	for (int i = 0; i < point.Size(); i++)
		if (std::isinf(point[i])) return true;
	return false;
}
