#include <chrono>
#include <cstring>
#include <random>

#include "functions.hpp"

int getRandomNumber(int from, int to)
{
   try {
      if (from > to)
         throw std::runtime_error(
           "Incorrect couple 'from - to' for generating random numbers");
   } catch (const std::runtime_error& err) {
      std::cerr << err.what() << '\n';
      exit(1);
   }

   unsigned int now = static_cast< unsigned >(
     std::chrono::high_resolution_clock::now().time_since_epoch().count() %
     10000);
   std::mt19937 engine(now);
   std::uniform_int_distribution< int > random(from, to);

   return random(engine);
}

double round(double number, int8_t dds)
{
   if (dds < 0)
      throw std::invalid_argument("round: dds should be positive.");

   uint mult = 1;
   while (dds != 0) {
      mult *= 10;
      --dds;
   }

   return std::round(number * mult) / static_cast< double >(mult);
}

bool areEqual(double a, double b, int8_t dds)
{
   if (dds < 0)
      throw std::invalid_argument("areEqual: dds should be positive.");

   return round(a, dds) == round(b, dds);
}

bool areEqual(double a, double b, double precision)
{
   if (precision < 0.0)
      throw std::invalid_argument("areEqual: precision should be positive.");
   return std::fabs(a - b) <= precision;
}

int getNumberDigits(int number)
{
   int numberDigits = 0;
   while (number != 0) {
      ++numberDigits;
      number /= 10;
   }
   return numberDigits;
}

bool isZero(double a)
{
    return fabs(a) <= std::numeric_limits<double>::epsilon();
}

int sign(double a)
{
    if (isZero(a)) return 0;
    else if (a < 0) return -1;
    else return 1;
}