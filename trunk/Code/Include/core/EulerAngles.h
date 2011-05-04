#pragma once

/// @file   core\EulerAngles.h
/// @brief  euler angles representation

#include "core\Class.h"
#include <d3dx9.h>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////

struct Vector;

///////////////////////////////////////////////////////////////////////////////

/**
 * Mathematical orientation - expressed by three angles to the respective axis
 * of the Cartesian system. The angular value is expressed in radians.
 */
struct EulerAngles : public RTTIObject
{
   DECLARE_RTTI_STRUCT

   /// these values are stored in degrees
   float yaw, pitch, roll;

   /**
    * Default constructor.
    */
   EulerAngles();

   /**
    * Constructor.
    *
    * @param yaw       rotation around OY axis
    * @param pitch     rotation around OX axis
    * @param roll      rotation around OZ axis
    */
   EulerAngles(float yaw, float pitch, float roll);

   /**
    * Constructor that calculates an angles between the two vectors.
    *
    * @param vec1
    * @param vec2
    */
   EulerAngles(const Vector& vec1, const Vector& vec2);

   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   bool operator==(const EulerAngles& rhs) const;
   bool operator!=(const EulerAngles& rhs) const;
   EulerAngles& operator=(const EulerAngles& rhs);
   EulerAngles operator+(const EulerAngles& rhs) const;
   EulerAngles& operator+=(const EulerAngles& rhs);
   EulerAngles operator-(const EulerAngles& rhs) const;
   EulerAngles& operator-=(const EulerAngles& rhs);
   EulerAngles operator*(const EulerAngles& rhs) const;
   EulerAngles& operator*=(const EulerAngles& rhs);
   EulerAngles operator/(const EulerAngles& rhs) const;
   EulerAngles& operator/=(const EulerAngles& rhs);

   EulerAngles operator+(float val) const;
   EulerAngles& operator+=(float val);
   EulerAngles operator-(float val) const;
   EulerAngles& operator-=(float val);
   EulerAngles operator*(float val) const;
   EulerAngles& operator*=(float val);
   EulerAngles operator/(float val) const;
   EulerAngles& operator/=(float val);

   operator D3DXQUATERNION() const;

   // -------------------------------------------------------------------------
   // Operations
   // -------------------------------------------------------------------------

   /**
    * Returns a normalized version of the orientation.
    * Angles of the normalized orientation are in range[0, 360)
    */
   EulerAngles normalized() const;

   /**
    * Normalizes this orientation.
    * Angles of the normalized orientation are in range[0, 360)
    */
   EulerAngles& normalize();

   /**
    * This method writes the contents of the orientation to the specified 
    * output stream.
    *
    * @param stream     output stream
    * @param rhs        orientation description of which we want to output
    */
   friend std::ostream& operator<<(std::ostream& stream, const EulerAngles& rhs);

   /**
    * Static method converting radians to degrees.
    *
    * @param radians    radian angular value
    * @return           degree angular value
    */
   static float toDegrees(float radians);

   /**
    * Static method converting radians to degrees.
    *
    * @param degrees    degree angular value
    * @return           radian angular value
    */
   static float toRadians(float degrees);
};

///////////////////////////////////////////////////////////////////////////////

#define RAD2DEG(radians) EulerAngles::toDegrees(radians)
#define DEG2RAD(degrees) EulerAngles::toRadians(degrees)

///////////////////////////////////////////////////////////////////////////////
