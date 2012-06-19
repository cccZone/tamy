/// @file   core\Quaternion.h
/// @brief  quaternion representation
#pragma once

#include <iostream>


///////////////////////////////////////////////////////////////////////////////

struct EulerAngles;
class OutStream;
class InStream;
struct Vector;

///////////////////////////////////////////////////////////////////////////////

/**
 * Quaternion representation.
 */
struct Quaternion
{
   union
   {
      float q[4];

      struct 
      {
         float x, y, z, w;
      };
   };

   // -------------------------------------------------------------------------
   // Static values
   // -------------------------------------------------------------------------
   static Quaternion     IDENTITY;

   /**
    * Default constructor.
    */
   Quaternion();

   /**
    * Constructor.
    *
    * @param x, y, z, w       quaternion coordinates
    */
   Quaternion( float x, float y, float z, float w );

   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   Quaternion& operator=( const Quaternion& rhs );
   bool operator==( const Quaternion& rhs ) const;
   bool operator!=( const Quaternion& rhs ) const;

   // -------------------------------------------------------------------------
   // Setters
   // -------------------------------------------------------------------------

   /**
    * this = q1 + q2
    */
   Quaternion& setAdd( const Quaternion& q1, const Quaternion& q2 );

   /**
    * this = q1 - q2
    */
   Quaternion& setSub( const Quaternion& q1, const Quaternion& q2 );

   /**
    * this = q1 * q2
    */
   Quaternion& setMul( const Quaternion& q1, const Quaternion& q2 );

   /**
    * this = this + q2
    */
   Quaternion& add( const Quaternion& rhs );

   /**
    * this = this - q2
    */
   Quaternion& sub( const Quaternion& rhs );

   /**
    * this = this * q2.
    */
   Quaternion& mul( const Quaternion& rhs );

   /**
    * this = -this;
    */
   Quaternion& neg();

   /**
    * Sets the inverse of the specified quaternion, which accounts for the same rotation, only in the opposite
    * direction.
    */
   Quaternion& setConjugated( const Quaternion& rhs );

   /**
   * Inverts the specified quaternion, which accounts for the same rotation, only in the opposite
   * direction.
    */
   Quaternion& conjugate();

   /**
    * Converts EulerAngles into a quaternion
    *
    * @param angles           Euler angles
    */
   Quaternion& setFromEulerAngles( const EulerAngles& angles );

   /**
    * Creates a quaternion from an axis and an angle.
    * The axis vector NEEDS TO BE NORMALIZED
    *
    * @param axis
    * @param angle      expressed in RADIANS
    */
   Quaternion& setAxisAngle( const Vector& axis, float angle );

   /**
    * Creates a quaternion that describes the shortest rotation that would
    * transform one vector into another vector. The vectors NEED TO BE NORMALIZED.
    *
    * @param v1         start vector
    * @param v2         end vector
    */
   Quaternion& setFromShortestRotation( const Vector& v1, const Vector& v2 );

   /**
    * Creates a new quaternion that is a result of a spherical interpolation
    * between two specified quaternions.
    *
    * @param a             start quaternion
    * @param b             end quaternion
    * @param t             interpolation distance
    */
   Quaternion& setSlerp( const Quaternion& a, const Quaternion& b, float t );

   /**
    * Normalizes the quaternion.
    */
   Quaternion& normalize();

   /**
    * Returns the axis this quaternion rotates about. 
    * 
    * CAUTION: This method doesn't work for identity quaternion,
    * because in that case there's no axis and angle defined to begin with.
    *
    * @param outAxis
    */
   void getAxis( Vector& outAxis ) const;

   /**
    * Returns the angle this quaternion rotates about ( in RADIANS ).
    *
    * CAUTION: This method doesn't work for identity quaternion,
    * because in that case there's no axis and angle defined to begin with.
    */
   float getAngle() const;

   /**
    * Removes the component that rotates this quaternion about the specified axis.
    */
   Quaternion& removeAxisComponent( const Vector& axis );

   /**
    * Decomposes a quaternion about the specified axis, returning the angle that it would
    * rotate about and the remaining quaternion.
    *
    * WARNING: The specified axis MUST BE A UNIT-LENGTH VECTOR.
    *
    * @param decompositionAxis
    * @param outRemainingQuaternion
    * @return decomposed rotation angle ( in RADIANS )
    */
   float decompose( const Vector& decompositionAxis, Quaternion& outRemainingQuaternion ) const;

   /**
    * Rotates a vector.
    *
    * CAUTION: don't specify the same vector twice here, as the results will be unpredictable.
    *
    * @param inVec      vector to transform
    * @param outVec     transformed vector
    */
   void transform( const Vector& inVec, Vector& outVec ) const;

   /**
    * This method writes the description of the quaternion to the specified 
    * output stream.
    *
    * @param stream     output stream
    * @param rhs        vector description of which we want to output
    */
   friend std::ostream& operator<<(std::ostream& stream, const Quaternion& rhs);

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend OutStream& operator<<( OutStream& serializer, const Quaternion& rhs );
   friend InStream& operator>>( InStream& serializer, Quaternion& rhs );
};

///////////////////////////////////////////////////////////////////////////////