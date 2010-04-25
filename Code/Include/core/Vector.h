#pragma once

/// @file   core\Vector.h
/// @brief  math vector

#include "core\Class.h"
#include <d3dx9.h>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////

/**
 * Mathematical 3d vector.
 */
struct Vector : public RTTIObject
{
   DECLARE_RTTI_STRUCT

   union
   {
      float v[3];

      struct 
      {
         float x, y, z;
      };
   };

   /**
    * Default constructor.
    */
   Vector();

   /**
    * Constructor.
    *
    * @param _x, _y, _z    coordinates
    */
   Vector(float _x, float _y, float _z);

   /**
    * Constructor.
    *
    * @param dxVec         DirectX vector
    */
   Vector(const D3DXVECTOR3& dxVec);

   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   inline operator D3DXVECTOR3 () const { return D3DXVECTOR3(x, y, z); }
   Vector operator-() const;
   bool operator==(const Vector& rhs) const;
   bool operator!=(const Vector& rhs) const;
   Vector& operator=(const Vector& rhs);
   Vector operator+(const Vector& rhs) const;
   Vector& operator+=(const Vector& rhs);
   Vector operator-(const Vector& rhs) const;
   Vector& operator-=(const Vector& rhs);
   Vector operator*(float val) const;
   Vector& operator*=(float val);

   // -------------------------------------------------------------------------
   // Operations
   // -------------------------------------------------------------------------

   /**
    * Calculate a dot product with another vector.
    *
    * @param rhs     other vector we want to use in the calculations
    */
   float dot(const Vector& rhs) const;

   /**
    * Calculate a cross product with another vector.
    *
    * @param rhs     other vector we want to use in the calculations
    */
   Vector cross(const Vector& rhs) const;

   /**
    * Returns a normalized version of this vector.
    */
   Vector normalized() const;

   /**
    * Normalizes this vector (in place).
    */
   Vector& normalize();

   /**
    * Returns the length of this vector.
    */
   float length() const;

   /**
    * Returns the squared length of this vector.
    */
   float lengthSq() const;

   /**
    * This method writes the contents of the vector to the specified 
    * output stream.
    *
    * @param stream     output stream
    * @param rhs        vector description of which we want to output
    */
   friend std::ostream& operator<<(std::ostream& stream, const Vector& rhs);
};

///////////////////////////////////////////////////////////////////////////////
