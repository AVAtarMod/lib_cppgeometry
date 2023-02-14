cmake_minimum_required(VERSION 3.0)
project(lib_cppgeometry)

# Always include the source and build directories in the include path.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# create the static library
add_library(shared STATIC
Angle.cpp          CircleArc.cpp  Circle.cpp
ComplexNumber.cpp  Line.cpp
LineSegment.cpp    Point.cpp      Quadrilateral.cpp functions.cpp)
