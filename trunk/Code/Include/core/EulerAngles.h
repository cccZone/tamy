/// @file   core\EulerAngles.h
/// @brief  Euler angles representation
#pragma once

#include <iostream>


///////////////////////////////////////////////////////////////////////////////

struct Vector;
struct Quaternion;
class OutStream;
class InStream;

///////////////////////////////////////////////////////////////////////////////

/**
 * Mathematical orientation - expressed by three angles to the respective axis
 * of the Cartesian system. The angular value is expressed in radians.
 */
struct EulerAngles
{
   /// these values are stored in degrees
   float yaw, pitch, roll;

   /**
    * Default constructor.
    */
   EulerAngles();

   /**
    * Constructor.
    *
    * @param yaw       rotation around OY axis ( degrees )
    * @param pitch     rotation around OX axis ( degrees )
    * @param roll      rotation around OZ axis ( degrees )
    */
   EulerAngles( float yaw, float pitch, float roll );

   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   bool operator==(const EulerAngles& rhs) const;
   bool operator!=(const EulerAngles& rhs) const;
   EulerAngles& operator=(const EulerAngles& rhs);

   // <math.todo> replace those operators with methods
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
   
  
   // -------------------------------------------------------------------------
   // Operations
   // -------------------------------------------------------------------------

   /**
    * Constructor that calculates the shortest rotation angle that transforms vec1 into vec2.
    *
    * @param vec1
    * @param vec2
    */
   EulerAngles& setFromShortestRotation( const Vector& vec1, const Vector& vec2 );

   /**
    * Constructor that converts a quaternion to Euler angles.
    *
    * CAUTION: This method is HIGHLY unstable in terms of produced results.
    * and as a matter of fact it should not be used for any computations
    * that require stability.
    *
    * As EulerAngles will mostly be used in the editor, to display 
    * the angles in a human readable form, and Quaternions will be used anywhere else,
    * I suggest that all applicable editor data be stored as EulerAngles and then
    * converted to Quaternion, as that method is highly stable in terms of produced results.
    *
    * @param quat
    */
   EulerAngles& setFromQuaternion( const Quaternion& quat );

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

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend OutStream& operator<<( OutStream& serializer, const EulerAngles& rhs );
   friend InStream& operator>>( InStream& serializer, EulerAngles& rhs );
};

///////////////////////////////////////////////////////////////////////////////
