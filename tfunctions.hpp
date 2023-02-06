/**
 * @file tfunctions.hpp
 * @author Grigory Stupnikov (gs.obr@ya.ru)
 * @brief Namespaces and functions with task implementation code
 * @version 0.1
 * @date 2022-07-14
 *
 * @copyright Copyright © 2022 Grigory Stupnikov. All rights reserved. Licensed
 * under GNU GPLv3. See https://opensource.org/licenses/GPL-3.0.
 */
#ifndef COURSEWORK_4_1_TFUNCTIONS_HPP
#define COURSEWORK_4_1_TFUNCTIONS_HPP

#include "ComplexNumber.hpp"

#include <tuple>

using clineSegment_t = std::pair< ComplexNumber, ComplexNumber >;

struct ProgramOptions
{
   enum OutputStyle
   {
      sDEBUG, // Plain, no output except result, useful for debug
      sRICH  // Plain, print messages to user
   } outputStyle = sRICH;
};

/**
 * @brief Intersect of 2 line segments
 * @return ComplexNumber, intersection point of lines
 */
ComplexNumber intersect(const clineSegment_t& first, clineSegment_t second);

bool isPointBelongsSegment(const clineSegment_t& segment, ComplexNumber point);

/**
 * @brief Print message. It prints only numbers if options.outputStyle == DEBUG.
 * Passing non null-terminated string is UB, don't do this!
 * Supported format specifiers:
 * %N - complex number (ComplexNumber*)
 * %s - string (const char*)
 * @param ... data to print
 */
void printMessage(const ProgramOptions& options, const char* format, ...);

void printNumbers(const ProgramOptions& options, const ComplexNumber numbers[],
                  const std::string labels[], const size_t amount);

namespace task1 {
   void readTriangleFromUser(ComplexNumber arr[3], const std::string labels[3],
                             const ProgramOptions& options, int& returnCode);
   void solve(int& returnCode, const ProgramOptions& options);
}

namespace task2 {
   void readNumbersFromUser(ComplexNumber arr[4], const std::string labels[4],
                            const ProgramOptions& options, int& returnCode);
   void solve(int& returnCode, const ProgramOptions& options);
}
namespace task3 {
   void solve(int& returnCode, const ProgramOptions& options);
}
namespace task4 {
   void readNumbersFromUser(ComplexNumber arr[5], const std::string labels[5],
                            const ProgramOptions& options, int& returnCode);

   void solve(int& returnCode, const ProgramOptions& options);
}
namespace task5 {
   void readNumbersFromUser(ComplexNumber arr[4], const std::string labels[4],
                            const ProgramOptions& options, int& returnCode);
   void solve(int& returnCode, const ProgramOptions& options);
}
namespace task6 {
   void readNumbersFromUser(ComplexNumber arr[4], const std::string labels[4],
                            const ProgramOptions& options, int& returnCode);
   void solve(int& returnCode, const ProgramOptions& options);
}
#endif // COURSEWORK_4_1_TFUNCTIONS_HPP
