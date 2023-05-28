#ifndef GEOMETRY_LIB_ANGLE_HPP
#define GEOMETRY_LIB_ANGLE_HPP

class Angle
{
  private:
   /**
    * @brief Value in degrees.
    */
   double _degrees;

  public:
   enum Type
   {
      /**
       * @brief Value between 0 and 360
       */
      Positive,
      /**
       * @brief Value between -360 and 360
       */
      Any,
      /**
       * @brief Value between -90 and 90
       */
      RightArea
   };
   /**
    * @brief Construct a new Angle object by degrees
    *
    * @param value an angle in degrees. Must be between 0 and 360
    * @param type angle type (see Type description)
    */
   Angle(double value = 0.0, Type type = Any);
   /**
    * @brief Construct a new Angle object by degrees
    *
    * @param value an angle in degrees. Must be between min and max
    */
   Angle(double min, double value, double max);

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
    * @param value Number that will divides `this` angle
    */
   Angle operator/(const double& number) const;

   /**
    * @brief Return value of `this` angle in degrees
    */
   double degrees() const { return _degrees; }

   static Angle fullAngle();
};
bool operator<(const double& degreeAngle, const Angle& other);
#endif // GEOMETRY_LIB_ANGLE _HPP
