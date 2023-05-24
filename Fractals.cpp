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

float fun(float a)
{
   const float n = -100, c = 0.97;
   return 0.5 + atan(n * a - c * n) / M_PI;
}

RGB Fractals::newColorMandelbrot(const ComplexNumber& cn,
                                 int iterations, int max_iterations)
{
   if (iterations == -1)
      return RGB(0, 0, 0);
   float a = (max_iterations - iterations) / (float)max_iterations;
   //a *= cn.Mod2();
   double t = 240 * a;
   return HSVtoRGB(
     HSV(t, 80, 100 * (fun(a))));
}

int Fractals::numIterations(ComplexNumber& z,
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
         z = z * z * z + c;
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
   ComplexNumber cn(p), z;

   for (i = 0; i < height_px; i++) {
      cn = cn - ComplexNumber(0, h);
      for (j = 0; j < width_px; j++) {
         z = cn + ComplexNumber(j * h, 0);
         ans[i][j] = newColorMandelbrot(
           z, numIterations(z, max_iterations), max_iterations);
      }
   }
   return ans;
}