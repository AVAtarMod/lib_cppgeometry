cmake_minimum_required(VERSION 3.0)
project(lib_cppgeometry)

# Always include the source and build directories in the include path.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# create the static library
add_library(shared STATIC Angle.cpp      ComplexNumber.cpp  Point.cpp
CircleArc.cpp  Line.cpp           Quadrilateral.cpp
Circle.cpp     LineSegment.cpp    tfunctions.cpp functions.cpp)
