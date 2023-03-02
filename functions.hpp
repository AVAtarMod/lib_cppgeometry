#ifndef CPPLIB_FUNCTIONS_HPP
#define CPPLIB_FUNCTIONS_HPP

#include <cmath>
#include <iostream>
#include <limits>

using uint = unsigned int; 

int getRandomNumber(int from, int to);

/**
 * @brief Round double to specified digits after decimal separator
 *
 * @param number source number
 * @param dds amount digits after decimal separator. Should be positive
 * @return double - Rounded number
 */
double round(double number, int8_t dds = 0);
/**
 * @brief Compare 2 numbers with specified precision
 *
 * @param a first number
 * @param b second number
 * @param dds Compare precision - digits after decimal separator. Should be
 * positive
 * @return true if the numbers are equal, false otherwise
 */
bool areEqual(double a, double b, int8_t dds = 0);
/**
 * @brief Compare 2 numbers with specified precision
 *
 * @param a first number
 * @param b second number
 * @param precision Compare precision. Should be positive
 * @return true if the numbers are differs lesser than specified precision
 * value, false otherwise
 */
bool areEqual(double a, double b, double precision = 0.01);

template< class T >
T power(T a, uint power)
{
   if (power == 0)
      return 1;
   T result = a;
   while (power != 1) {
      result *= a;
      --power;
   }
   return result;
}

template< class T >
typename std::enable_if< !std::numeric_limits< T >::is_integer, bool >::type almost_equal(
  T x, T y, int ulp = 2)
{
   // the machine epsilon has to be scaled to the magnitude of the values
   // used and multiplied by the desired precision in ULPs (units in the
   // last place)
   return std::fabs(x - y) <=
            std::numeric_limits< T >::epsilon() * std::fabs(x + y) * ulp
          // unless the result is subnormal
          || std::fabs(x - y) < std::numeric_limits< T >::min();
}

int getNumberDigits(int number);

/**
 * @brief Checks if the number is zero
 */
bool isZero(double a);

/**
 * @brief Calculates the sign of a number
 *
 * @return 1 if the number is greater than zero,
 * 0 if the number is zero,
 * -1 if the number is less than zero
 */
int sign(double a);

#endif // CPPLIB_FUNCTIONS_HPP
