cmake_minimum_required(VERSION 3.0)
project(lib_cppgeometry)

# Always include the source and build directories in the include path.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# create the static library
add_library(shared STATIC
Angle.cpp          CircleArc.cpp  Circle.cpp
ComplexNumber.cpp  Line.cpp
LineSegment.cpp    Point.cpp      Quadrilateral.cpp functions.cpp Polygon.cpp Graph.cpp Fractals.cpp
Curve.cpp)
