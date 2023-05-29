#ifndef GEOMETRY_LIB_FRACTALS_HPP
#define GEOMETRY_LIB_FRACTALS_HPP

#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <iostream>
#include <vector>

#include "ComplexNumber.hpp"
#include "Point.hpp"
#include "functions.hpp"

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

const double _persistence = 1.5;

class Fractals
{
  private:
   static RGB newColorMandelbrot(const ComplexNumber& cn,
                                 int max_iterations);
   static int numIterationsMandelbrot(ComplexNumber& z,
                                      int max_iterations);

   static RGB newColorNewton(const ComplexNumber& cn);
   static int numIterationsNewton(ComplexNumber& z);

   static void heightsPlasma(
     std::vector<std::vector<double>>& heights);
   static RGB heightToRGB(double height);
   static void diamond(std::vector<std::vector<double>>& heights,
                       const int& iter, const int& i, const int& j,
                       const int& width, const int& width2);
   static void square(std::vector<std::vector<double>>& heights,
                      const int& iter, const int& i, const int& j,
                      const int& width, const int& width2);
   static bool insideSquare(const int& i, const int& j,
                            const int& size);
   static double getRandNum(int iter);

   static void brokenHeightsPlasma(
     std::vector<std::vector<double>>& heights);
   static RGB brokenHeightToRGB(double height);
   static void brokenDiamond(
     std::vector<std::vector<double>>& heights, const int& i,
     const int& j, const int& width, const int& width2);
   static void brokenSquare(std::vector<std::vector<double>>& heights,
                            const int& i, const int& j,
                            const int& width, const int& width2);

  public:
   struct Area
   {
      size_t width_px;
      size_t height_px;
      double min_x, max_x;
      double min_y, max_y;
      Area(size_t width_px, size_t height_px,
           std::pair<double, double> x_minmax,
           std::pair<double, double> y_minmax)
      {
         this->width_px = width_px;
         this->height_px = height_px;
         min_x = x_minmax.first;
         max_x = x_minmax.second;
         min_y = y_minmax.first;
         max_y = y_minmax.second;
      }
   };
   enum class GeometricFractalType
   {
      KOCH_SNOWFLAKE,
      PYTHAGORAS_TREE_CLASSIC,
      PYTHAGORAS_TREE_NAKED
   };
   static std::vector<Point> geometricFractal(const Point& p,
                                              const Area& area,
                                              GeometricFractalType t);
   /**
    * @brief Creates a Mandelbrot set
    *
    * @param p: the point of the upper left corner of the area in
    * local coordinates
    * @param width_px: area width in pixels
    * @param height_px: area height in pixels
    * @param width: area width in local coordinates
    * @param height: area height in local coordinates
    * @param max_iterations: the maximum number of checks for a point
    * belonging to the set
    */
   static std::vector<std::vector<RGB>> mandelbrotSet(
     const Point& p, int width_px, int height_px, double width,
     double height, int max_iterations = 1000);
   /**
    * @brief Creates a Newton Fractal
    *
    * @param p: the point of the upper left corner of the area in
    * local coordinates
    * @param width_px: area width in pixels
    * @param height_px: area height in pixels
    * @param width: area width in local coordinates
    * @param height: area height in local coordinates
    */
   static std::vector<std::vector<RGB>> NewtonFractal(const Point& p,
                                                      int width_px,
                                                      int height_px,
                                                      double width,
                                                      double height);
   /**
    * @brief Creates a Plasma Fractal
    *
    * @param n: determines the size of the image
    * 2^n + 1 = size
    */
   static std::vector<std::vector<RGB>> plasmaFractal(int n);
   /**
    * @brief Creates a broken Plasma Fractal
    *
    * @param n: determines the size of the image
    * 2^n + 1 = size
    */
   static std::vector<std::vector<RGB>> brokenPlasmaFractal(int n);
};

#endif // GEOMETRY_LIB_FRACTALS_HPP
