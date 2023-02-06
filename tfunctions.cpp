#include "tfunctions.hpp"
#include "Line.hpp"
#include "Quadrilateral.hpp"

#include <cmath>
#include <cstdarg>
#include <cstring>
#include <functions.hpp>
#include <unordered_map>

void printElementRichStyle(const char* specifier, const void* data);
void printElementUnixStyle(const char* specifier, const void* data);
void printElement(const ProgramOptions& options, const char* specifier,
                  const void* data);
bool isValidSpecifier(const std::string& specifier);
const void* extractElipsisElement(va_list* elipsis,
                                  const std::string& specifier);

const size_t specifiersAmount = 2;

enum class ElementType
{
   ComplexNumber,
   String
};

const std::unordered_map< std::string, ElementType > specifiers = {
   { "%N", ElementType::ComplexNumber },
   { "%s", ElementType::String }
};

ComplexNumber intersect(const clineSegment_t& first, clineSegment_t second)
{
   return ComplexNumber { Line::intersect(Line(first.first, first.second),
                                          Line(second.first, second.second)) };
}

bool isPointBelongsSegment(const clineSegment_t& segment, ComplexNumber cpoint)
{
   const ComplexNumber &a = segment.first, &b = segment.second;
   Line line(a, b);

   double imMax = std::max(a.Im(), b.Im()), imMin = std::min(a.Im(), b.Im());
   double reMax = std::max(a.Re(), b.Re()), reMin = std::min(a.Re(), b.Re());
   bool isPointInBounds = (imMin <= cpoint.Im() && cpoint.Im() <= imMax) &&
                          (reMin <= cpoint.Re() && cpoint.Re() <= reMax);

   return line.isBelongs(static_cast< Point >(cpoint)) && isPointInBounds;
}

void task1::readTriangleFromUser(ComplexNumber arr[3],
                                 const std::string labels[3],
                                 const ProgramOptions& options, int& returnCode)
{
   const size_t labelsCount = 3;

   // clang-format off
   
   ComplexNumber  &a = arr[0],
                  &b = arr[1],
                  &c = arr[2];
   // clang-format on

   bool isTriangle = false;
   while (!isTriangle) {
      printMessage(options, "Enter coordinates of triangle's points:\n");
      if (std::cin.fail()) {
         if (std::cin.eof()) {
            std::cerr << "User input was canceled. Aborting...\n";
            return;
         }
         std::cin.ignore();
         std::cin.clear();
      }

      for (size_t i = 0; i < labelsCount; i++) {
         printMessage(options, (' ' + labels[i] + ": ").c_str());
         std::cin >> arr[i];
         arr[i] = ComplexNumber::round(arr[i], 2);
      }

      isTriangle = !Line::isOnSameLine(a, b, c);
      if (!isTriangle)
         std::cerr << "Incorrect a,b,c. Must be points of the triangle ABC\n";
   }
}

void task2::readNumbersFromUser(ComplexNumber arr[4],
                                const std::string labels[4],
                                const ProgramOptions& options, int& returnCode)
{
   returnCode = 0;
   const size_t labelsCount = 4;

   // clang-format off
   ComplexNumber  &a = arr[0],
                  &b = arr[1];
   // clang-format on

   bool isEqual = true;
   while (isEqual) {
      printMessage(
        options,
        "Enter coordinates of a,b,n,t points (must conform x^2+y^2 = 1, t between a and b):\n");

      if (std::cin.fail()) {
         if (std::cin.eof()) {
            std::cerr << "User input was canceled. Aborting...\n";
            returnCode = 1;
            return;
         }
         std::cin.ignore();
         std::cin.clear();
      }

      for (size_t i = 0; i < labelsCount; i++) {
         printMessage(options, (' ' + labels[i] + ": ").c_str());
         std::cin >> arr[i];
         arr[i] = ComplexNumber::round(arr[i], 2);
      }

      isEqual = a == b;
      if (isEqual)
         std::cerr << "Incorrect a,b. Must be not equal\n";
   }
}

void task4::readNumbersFromUser(ComplexNumber arr[5],
                                const std::string labels[5],
                                const ProgramOptions& options, int& returnCode)
{
   returnCode = 0;
   const size_t labelsCount = 5;

   // clang-format off
   ComplexNumber  &a = arr[0],
                  &b = arr[1],
                  &c = arr[2],
                  &a1 = arr[3],
                  &b1 = arr[4];
   // clang-format on

   bool isTriangle = false, isValidA1 = false, isValidB1 = false;
   while (!(isTriangle && isValidA1 && isValidB1)) {
      printMessage(options, "Enter coordinates of a,b,c,a1,b1 points:\n");
      if (std::cin.fail()) {
         if (std::cin.eof()) {
            std::cerr << "User input was canceled. Aborting...\n";
            returnCode = 1;
            return;
         }
         std::cin.ignore();
         std::cin.clear();
      }
      for (size_t i = 0; i < labelsCount; i++) {
         printMessage(options, (' ' + labels[i] + ": ").c_str());
         std::cin >> arr[i];
         arr[i] = ComplexNumber::round(arr[i], 2);
      }

      isTriangle = !Line::isOnSameLine(a, b, c);
      isValidA1 = isPointBelongsSegment({ b, c }, a1);
      isValidB1 = isPointBelongsSegment({ a, c }, b1);

      if (!isTriangle)
         std::cerr << "Incorrect a,b,c. Must be points of the triangle ABC\n";
      if (!isValidA1)
         std::cerr
           << "The a1 is incorrect. Must belong to segment of line BC\n";
      if (!isValidB1)
         std::cerr
           << "The b1 is incorrect. Must belong to segment of line AC\n";
   }
}

void task5::readNumbersFromUser(ComplexNumber arr[4],
                                const std::string labels[4],
                                const ProgramOptions& options, int& returnCode)
{
   returnCode = 0;
   const size_t labelsCount = 4;

   printMessage(
     options,
     "Enter coordinates of a,b,c,d points (must be points of quadrilateral):\n");
   bool validPoints = false;
   while (!validPoints) {
      if (std::cin.fail()) {
         if (std::cin.eof()) {
            std::cerr << "User input was canceled. Aborting...\n";
            returnCode = 1;
            return;
         }
         std::cin.ignore();
         std::cin.clear();
      }

      for (size_t i = 0; i < labelsCount; i++) {
         printMessage(options, (' ' + labels[i] + ": ").c_str());
         std::cin >> arr[i];
         arr[i] = ComplexNumber::round(arr[i], 2);
      }
      try {
         Quadrilateral tmp(static_cast< Point >(arr[0]),
                           static_cast< Point >(arr[1]),
                           static_cast< Point >(arr[2]),
                           static_cast< Point >(arr[3]));
      } catch (const std::invalid_argument&) {
         std::cerr
           << "Cannot construct quadrilateral by a,b,c,d. You should enter valid points.\n";
         continue;
      }
      validPoints = true;
   }
}

void task6::readNumbersFromUser(ComplexNumber arr[4],
                                const std::string labels[4],
                                const ProgramOptions& options, int& returnCode)
{
   returnCode = 0;
   const size_t labelsCount = 4;

   printMessage(
     options,
     "Enter coordinates of a,b,c,d points (must be points of quadrilateral):\n");

   if (std::cin.fail()) {
      if (std::cin.eof()) {
         std::cerr << "User input was canceled. Aborting...\n";
         returnCode = 1;
         return;
      }
      std::cin.ignore();
      std::cin.clear();
   }

   for (size_t i = 0; i < labelsCount; i++) {
      printMessage(options, (' ' + labels[i] + ": ").c_str());
      std::cin >> arr[i];
      arr[i] = ComplexNumber::round(arr[i], 2);
   }
}

void printElementUnixStyle(const ElementType& type, const void* data)
{
   const ComplexNumber* number;

   switch (type) {
      case ElementType::ComplexNumber:
         number = static_cast< const ComplexNumber* >(data);
         if (number) {
            const double re = number->Re(), im = number->Im();
            std::cout
              << ((std::abs(re) < std::numeric_limits< double >::epsilon())
                    ? 0
                    : re)
              << ' '
              << ((std::abs(im) < std::numeric_limits< double >::epsilon())
                    ? 0
                    : im);
         }
         break;

      case ElementType::String:
      default:
         break;
   }
}

void printElementRichStyle(const ElementType& type, const void* data)
{
   const ComplexNumber* number;
   const char* string;
   switch (type) {
      case ElementType::ComplexNumber:
         number = static_cast< const ComplexNumber* >(data);
         if (number)
            std::cout << *number;
         break;
      case ElementType::String:
         string = static_cast< const char* >(data);
         if (string)
            std::cout << string;
      default:
         break;
   }
}

void printElement(const ProgramOptions& options, const ElementType& type,
                  const void* data)
{
   switch (options.outputStyle) {
      case ProgramOptions::sDEBUG:
         printElementUnixStyle(type, data);
         break;
      case ProgramOptions::sRICH:
         printElementRichStyle(type, data);
         break;
      default:
         break;
   }
}

void printChar(const ProgramOptions& options, const char& c)
{
   switch (options.outputStyle) {
      case ProgramOptions::sRICH:
         std::cout << c;
         break;
      default:
         break;
   }
}

bool isValidSpecifier(const std::string& specifier)
{
   if (specifiers.find(specifier) != specifiers.cend())
      return true;
   else
      return false;
}

void printMessage(const ProgramOptions& options, const char* format, ...)
{
   va_list data;
   size_t formatLen = strlen(format);
   const void* element;

   va_start(data, format);
   for (size_t i = 0; i < formatLen; ++i) {
      switch (format[i]) {
         case '%':
            if (i < formatLen - 1) {
               const std::string specifier(&format[i], 2);
               if (isValidSpecifier(specifier)) {
                  element = extractElipsisElement(&data, specifier);
                  printElement(options, specifiers.at(specifier), element);
               }
               ++i;
            }
            break;
         default:
            printChar(options, format[i]);
            break;
      }
   }
   va_end(data);
}

void printNumbers(const ProgramOptions& options, const ComplexNumber numbers[],
                  const std::string labels[], const size_t amount)
{
   switch (options.outputStyle) {
      case ProgramOptions::sDEBUG:
         for (size_t i = 0; i < amount; i++) {
            printElementUnixStyle(ElementType::ComplexNumber, &numbers[i]);
            if (i < amount - 1)
               std::cout << ' ';
         }
         break;
      case ProgramOptions::sRICH:
         for (size_t i = 0; i < amount; i++) {
            std::cout << " " << labels[i] << ": ";
            printElementRichStyle(ElementType::ComplexNumber, &numbers[i]);
            if (i < amount - 1)
               std::cout << '\n';
         }
         break;
      default:
         break;
   }
}

const void* extractElipsisElement(va_list* elipsis,
                                  const std::string& specifier)
{
   switch (specifiers.at(specifier)) {
      case ElementType::ComplexNumber:
         return va_arg(*elipsis, ComplexNumber*);
      case ElementType::String:
         return va_arg(*elipsis, const char*);
      default:
         return nullptr;
   }
}
