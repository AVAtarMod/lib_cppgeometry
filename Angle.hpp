#ifndef COURSEWORK_4_1_ANGLE_HPP
#define COURSEWORK_4_1_ANGLE_HPP

class Angle
{
  private:
   /**
    * @brief Value in degrees. Must be between 0 and 360
    */
   double _degrees;

  public:
   /**
    * @brief Construct a new Angle object by degrees
    *
    * @param value an angle in degrees
    */
   Angle(double value = 0.0);

   bool operator==(const Angle& other) const;
   bool operator!=(const Angle& other) const;
   bool operator>(const Angle& other) const;
   bool operator<(const Angle& other) const;
   bool operator<=(const Angle& other) const;
   bool operator>=(const Angle& other) const;

   Angle operator+(const Angle& other) const;
   Angle operator-(const Angle& other) const;
   /**
    * @brief Divide angle value to number
    * 
    * @param value Number that will divides this angle
    */
   Angle operator/(const double& number) const;

   /**
    * @brief Return value of this angle in degrees
    */
   double degrees() const { return _degrees; }

   static Angle fullAngle();
};

#endif // COURSEWORK_4_1_ANGLE _HPP
