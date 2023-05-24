#include "Fractals.hpp"

//https://www.codespeedy.com/hsv-to-rgb-in-cpp/
RGB HSVtoRGB(const HSV& c)
{
   RGB ans;
   double s = c.S / 100;
   double v = c.V / 100;
   double C = s * v;
   double X = C * (1 - abs(fmod(c.H / 60.0, 2) - 1));
   double m = v - C;
   double r, g, b;
   if (c.H >= 0 && c.H < 60) {
      r = C, g = X, b = 0;
   } else if (c.H >= 60 && c.H < 120) {
      r = X, g = C, b = 0;
   } else if (c.H >= 120 && c.H < 180) {
      r = 0, g = C, b = X;
   } else if (c.H >= 180 && c.H < 240) {
      r = 0, g = X, b = C;
   } else if (c.H >= 240 && c.H < 300) {
      r = X, g = 0, b = C;
   } else {
      r = C, g = 0, b = X;
   }
   ans._red = (r + m) * 255;
   ans._green = (g + m) * 255;
   ans._blue = (b + m) * 255;
   return ans;
}

RGB Fractals::newColorMandelbrot(const ComplexNumber& cn, int max_iterations)
{
   ComplexNumber z = cn;
   int iterations = numIterationsMandelbrot(z, max_iterations);

   if (iterations == -1)
      return RGB(0, 0, 0);
   float a = (max_iterations - iterations) / (float)max_iterations;
   //a *= cn.Mod2();
   double t = 240 * a;
   const float n = -100, c = 0.97;
   return HSVtoRGB(
     HSV(t, 80, 100 * (0.5 + atan(n * a - c * n) / M_PI)));
}

int Fractals::numIterationsMandelbrot(ComplexNumber& z,
                            int max_iterations)
{
   double mod2;
   const double barrier = 4;
   ComplexNumber c(z);
   mod2 = c.Mod2();
   if (mod2 < barrier || isZero(mod2 - barrier))
      z = c;
   else
      return 0;
   for (int i = 1; i <= max_iterations; i++) {
      mod2 = z.Mod2();
      if (mod2 < barrier || isZero(mod2 - barrier))
         z = z * z + c;
      else
         return i;
   }
   return -1;
}

std::vector<std::vector<RGB>> Fractals::mandelbrotSet(
  const Point& p, int width_px, int height_px, double width,
  double height, int max_iterations)
{
   int i, j;
   std::vector<std::vector<RGB>> ans(height_px);
   for (i = 0; i < height_px; i++)
      ans[i] = std::vector<RGB>(width_px);

   double h = width / width_px;
   ComplexNumber cn(p);

   for (i = 0; i < height_px; i++) {
      cn = cn - ComplexNumber(0, h);
      for (j = 0; j < width_px; j++) {
         ans[i][j] = newColorMandelbrot(cn + ComplexNumber(j * h, 0),
                                        max_iterations);
      }
   }
   return ans;
}

RGB Fractals::newColorNewton(const ComplexNumber& cn)
{
   ComplexNumber z = cn;
   int iterations = numIterationsNewton(z);

   if (iterations == -1)
      return RGB(0, 0, 0);
   float a;
   if (iterations == 0)
      a = 0;
   else
      a = 1.0 / iterations;
   double t = 180 + 180 * z.Arg() / M_PI;
   const float n = -100, c = 0.06;
   return HSVtoRGB(
     HSV(t, 80, 5 + 100 * (0.5 + atan(n * a - c * n) / M_PI)));
}

int Fractals::numIterationsNewton(ComplexNumber& z)
{
   double mod2 = 1;
   const double max = 1e+10, min = 1e-10;
   ComplexNumber z_prev(z);
   bool c = true;
   int i;

   for (i = -1; c; i++) {
      if (mod2 < min)
         c = false;
      else if (mod2 > max) {
         c = false;
         i = -2;
      } else {
         z_prev = z;
         z = z - (z * z * z + ComplexNumber(1, 0)) /
                   (ComplexNumber(3, 0) * z * z);
         mod2 = (z - z_prev).Mod2();
      }
   }
   return i;
}

std::vector<std::vector<RGB>> Fractals::NewtonFractal(
  const Point& p, int width_px, int height_px, double width,
  double height)
{
   int i, j;
   std::vector<std::vector<RGB>> ans(height_px);
   for (i = 0; i < height_px; i++)
      ans[i] = std::vector<RGB>(width_px);

   double h = width / width_px;
   ComplexNumber cn(p);

   for (i = 0; i < height_px; i++) {
      cn = cn - ComplexNumber(0, h);
      for (j = 0; j < width_px; j++) {
         ans[i][j] = newColorNewton(cn + ComplexNumber(j * h, 0));
      }
   }
   return ans;
}