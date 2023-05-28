#include "Fractals.hpp"
#include "Circle.hpp"
#include "Polygon.hpp"
#include <list>

template<class T>
std::vector<T> vector_from(const std::list<T>& source)
{
   auto current = source.cbegin(), end = source.cend();
   std::vector<Point> v_result(source.size());
   for (size_t i = 0; current != end; ++i, ++current) {
      v_result[i] = *current;
   }
   return v_result;
}

// https://www.codespeedy.com/hsv-to-rgb-in-cpp/
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

RGB Fractals::newColorMandelbrot(const ComplexNumber& cn,
                                 int max_iterations)
{
   ComplexNumber z = cn;
   int iterations = numIterationsMandelbrot(z, max_iterations);

   if (iterations == -1)
      return RGB(0, 0, 0);
   float a = (max_iterations - iterations) / (float)max_iterations;
   // a *= cn.Mod2();
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

std::vector<std::vector<RGB>> Fractals::NewtonFractal(const Point& p,
                                                      int width_px,
                                                      int height_px,
                                                      double width,
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

std::vector<std::vector<RGB>> Fractals::plasmaFractal(int n)
{
   int i, size = (int)(pow(2, n) + 0.1) + 1;
   std::vector<std::vector<RGB>> ans(size);
   for (i = 0; i < size; i++)
      ans[i] = std::vector<RGB>(size);
   std::vector<std::vector<double>> heights(size);
   for (i = 0; i < size; i++)
      heights[i] = std::vector<double>(size);
   heights[0][0] = getRandNum(1);
   heights[0][size - 1] = getRandNum(1);
   heights[size - 1][0] = getRandNum(1);
   heights[size - 1][size - 1] = getRandNum(1);

   heightsPlasma(heights);
   for (i = 0; i < size; i++)
      std::transform(heights[i].begin(),
                     heights[i].end(),
                     ans[i].begin(),
                     [](double a) { return heightToRGB(a); });
   return ans;
}

void Fractals::heightsPlasma(
  std::vector<std::vector<double>>& heights)
{
   int i, j, ii;
   int size = heights.size(), iter = 1, width = size - 1,
       width2 = width / 2;
   while (width != 1) {
      for (i = 0; i < size - 1; i += width) {
         for (j = 0; j < size - 1; j += width) {
            diamond(heights, iter, i, j, width, width2);
            square(heights, iter, i, j, width, width2);
         }
         ii = i + width2;
         for (j = -width2; j < size; j += width)
            square(heights, iter, ii, j, width, width2);
      }
      for (j = 0; j < size - 1; j += width)
         square(heights, iter, i, j, width, width2);

      width = width / 2;
      width2 = width / 2;
      iter++;
   }
}

RGB Fractals::heightToRGB(double height)
{
   return HSVtoRGB(HSV(0, 0, (height + 1) * 50));
}

void Fractals::diamond(std::vector<std::vector<double>>& heights,
                       const int& iter, const int& i, const int& j,
                       const int& width, const int& width2)
{
   heights[i + width2][j + width2] =
     (heights[i][j] + heights[i][j + width] + heights[i + width][j] +
      heights[i + width][j + width]) /
       4 +
     getRandNum(iter);
}

std::list<Point> equaliteralTriangleByCenter(const Point& center,
                                             double side)
{
   std::list<Point> result;
   const double radius = (sqrt(3) / 3) * side;
   Circle circle(center, radius);
   const Angle start(90);
   result.push_back(circle.getPoint(start));
   result.insert(result.begin(), circle.getPoint(start + 120));
   result.push_back(circle.getPoint(Angle(start - 120)));
   return result;
}

std::vector<Point> fractalCochSnowflake(const Point& p,
                                        const Fractals::Area& area)
{
   Polygon area = Polygon::makeByArea({ area.min_x, area.max_x },
                                      { area.min_y, area.max_y });
   std::list<Point> result = equaliteralTriangleByCenter(p, 1);
   
   return vector_from(result);
}

std::vector<Point> fractalPythagorasTree(const Point& p,
                                         const Fractals::Area& area)
{
}

std::vector<Point> fractalPythagorasTreeNaked(
  const Point& p, const Fractals::Area& area)
{
}

std::vector<Point> Fractals::geometricFractal(const Point& p,
                                              const Area& area,
                                              GeometricFractalType t)
{
   switch (t) {
      case GeometricFractalType::KOCH_SNOWFLAKE:
         return fractalCochSnowflake(p, area);
      case GeometricFractalType::PYTHAGORAS_TREE_CLASSIC:
         return fractalPythagorasTree(p, area);
      case GeometricFractalType::PYTHAGORAS_TREE_NAKED:
         return fractalPythagorasTreeNaked(p, area);
   }
   return std::vector<Point>();
}

void Fractals::square(std::vector<std::vector<double>>& heights,
                      const int& iter, const int& i, const int& j,
                      const int& width, const int& width2)
{
   double sum = 0;
   int ii = i, jj = j;
   if (insideSquare(ii, jj, heights.size()))
      sum += heights[ii][jj];
   ii = i - width2, jj = j + width2;
   if (insideSquare(ii, jj, heights.size()))
      sum += heights[ii][jj];
   ii = i, jj = j + width;
   if (insideSquare(ii, jj, heights.size()))
      sum += heights[ii][jj];
   ii = i + width2, jj = j + width2;
   if (insideSquare(ii, jj, heights.size()))
      sum += heights[ii][jj];
   heights[i][j + width2] = sum / 4 + getRandNum(iter);
}

bool Fractals::insideSquare(const int& i, const int& j,
                            const int& size)
{
   return i >= 0 && i < size && j >= 0 && j < size;
}

double Fractals::getRandNum(int iter)
{
   double rn = (double)(rand()) * 2 / RAND_MAX - 1;
   int s = sign(rn);
   return s * pow(abs(rn), 1.0 / iter);
}

std::vector<std::vector<RGB>> Fractals::brokenPlasmaFractal(int n)
{
   int i, size = (int)(pow(2, n) + 0.1) + 1;
   std::vector<std::vector<RGB>> ans(size);
   for (i = 0; i < size; i++)
      ans[i] = std::vector<RGB>(size);
   std::vector<std::vector<double>> heights(size);
   for (i = 0; i < size; i++)
      heights[i] = std::vector<double>(size);
   heights[0][0] = (double)(rand()) / RAND_MAX;
   heights[0][size - 1] = (double)(rand()) / RAND_MAX;
   heights[size - 1][0] = (double)(rand()) / RAND_MAX;
   heights[size - 1][size - 1] = (double)(rand()) / RAND_MAX;

   brokenHeightsPlasma(heights);
   for (i = 0; i < size; i++)
      std::transform(heights[i].begin(),
                     heights[i].end(),
                     ans[i].begin(),
                     [](double a) { return brokenHeightToRGB(a); });
   return ans;
}

void Fractals::brokenHeightsPlasma(
  std::vector<std::vector<double>>& heights)
{
   int i, j, ii;
   int width = heights.size() - 1, width2 = width / 2;
   while (width != 1) {
      for (i = 0; i < heights.size() - 1; i += width) {
         for (j = 0; j < heights.size() - 1; j += width) {
            brokenDiamond(heights, i, j, width, width2);
            brokenSquare(heights, i, j, width, width2);
         }
         ii = i + width2;
         for (j = -width2; j < heights.size();
              j += width) // Never works. Maybe.
            brokenSquare(heights, ii, j, width, width2);
      }
      for (j = 0; j < heights.size() - 1; j += width)
         brokenSquare(heights, i, j, width, width2);

      width = width / 2;
      width2 = width / 2;
   }
}

void Fractals::brokenDiamond(
  std::vector<std::vector<double>>& heights, const int& i,
  const int& j, const int& width, const int& width2)
{
   heights[i + width2][j + width2] =
     (heights[i][j] + heights[i][j + width] + heights[i + width][j] +
      heights[i + width][j + width]) /
       4 +
     (double)(rand()) / RAND_MAX / 5.0 - 0.05;
}

void Fractals::brokenSquare(std::vector<std::vector<double>>& heights,
                            const int& i, const int& j,
                            const int& width, const int& width2)
{
   double sum = 0;
   int ii = i, jj = j;
   if (insideSquare(ii, jj, heights.size()))
      sum += heights[ii][jj];
   ii = i - width2, jj = j + width2;
   if (insideSquare(ii, jj, heights.size()))
      sum += heights[ii][jj];
   ii = i, jj = j + width;
   if (insideSquare(ii, jj, heights.size()))
      sum += heights[ii][jj];
   ii = i + width2, jj = j + width2;
   if (insideSquare(ii, jj, heights.size()))
      sum += heights[ii][jj];
   heights[i][j + width2] =
     sum / 4 + (double)(rand()) / RAND_MAX / 5.0 - 0.05;
}

RGB Fractals::brokenHeightToRGB(double height)
{
   return HSVtoRGB(HSV(0, 0, height * 100));
}
