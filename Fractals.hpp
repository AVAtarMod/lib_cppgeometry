#ifndef GEOMETRY_LIB_FRACTALS_HPP
#define GEOMETRY_LIB_FRACTALS_HPP


#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <iostream>
#include <vector>

#include "ComplexNumber.hpp"
#include "functions.hpp"
#include "Point.hpp"

struct RGB
{
   unsigned char _red;
   unsigned char _green;
   unsigned char _blue;
   RGB() {}
   RGB(unsigned char red, unsigned char green, unsigned char blue)
   {
      _red = red;
      _green = green;
      _blue = blue;
   }
};

struct HSV
{
   double H;
   double S;
   double V;
   HSV() {}
   HSV(double h, double s, double v)
   {
      if (h > 360)
         h = 360;
      else if (h < 0)
         h = 0;

      if (s > 100)
         s = 100;
      else if (s < 0)
         s = 0;

      if (v > 100)
         v = 100;
      else if (v < 0)
         v = 0;

      H = h;
      S = s;
      V = v;
   }
};

RGB HSVtoRGB(const HSV& c);

class Fractals
{
  public:
   static RGB newColorMandelbrot(const ComplexNumber& cn,
                                 int max_iterations);
   static int numIterationsMandelbrot(ComplexNumber& z,
                                      int max_iterations);
   static std::vector<std::vector<RGB>> mandelbrotSet(
     const Point& p, int width_px, int height_px, double width,
     double height, int max_iterations);

   static RGB newColorNewton(const ComplexNumber& cn);
   static int numIterationsNewton(ComplexNumber& z);
   static std::vector<std::vector<RGB>> NewtonFractal(const Point& p,
                                                      int width_px,
                                                      int height_px,
                                                      double width,
                                                      double height);
};

#endif // GEOMETRY_LIB_FRACTALS_HPP
