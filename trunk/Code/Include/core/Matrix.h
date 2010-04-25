#pragma once

/// @file   core\Matrix.h
/// @brief  matrix representation

#include "core\Class.h"
#include <d3dx9.h>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////

struct Vector;
struct EulerAngles;

///////////////////////////////////////////////////////////////////////////////

/**
 * Mathematical matrix - expressed in row major order expressed in left-handed 
 * Cartesian coordinate system.
 */
struct Matrix : public RTTIObject
{
   DECLARE_RTTI_STRUCT

   union
   {
      float m[4][4];

      struct  
      {
         float m11;
         float m12;
         float m13;
         float m14;
         float m21;
         float m22;
         float m23;
         float m24;
         float m31;
         float m32;
         float m33;
         float m34;
         float m41;
         float m42;
         float m43;
         float m44;
      };
   };

   static Matrix IDENTITY;

   /**
    * Default constructor.
    */
   Matrix();

   /**
    * Constructor.
    *
    * @param values  an array of 16 values
    */
   Matrix(const float* values);

   /**
    * Constructor.
    *
    * @param mtx    DirectX matrix this matrix will be based on.
    */
   Matrix(const D3DXMATRIX& mtx);

   /**
    * Constructor.
    *
    * @param angles  Euler angles that will construct a rotation matrix
    */
   Matrix(const EulerAngles& angles);

   /**
    * Constructor that creates a rotation matrix based on
    * an axis and a rotation around it.
    *
    * @param axis
    * @param angle
    */
   Matrix( const Vector& axis, float angle );

   /**
    * Constructor that creates a translation matrix based on
    * the specified vector
    *
    * @param translation
    */
   Matrix( const Vector& translation );

   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   bool operator==(const Matrix& rhs) const;
   bool operator!=(const Matrix& rhs) const;
   Matrix& operator=(const Matrix& rhs);
   Matrix operator+(const Matrix& rhs) const;
   Matrix& operator+=(const Matrix& rhs);
   Matrix operator-(const Matrix& rhs) const;
   Matrix& operator-=(const Matrix& rhs);
   Matrix operator*(const Matrix& rhs) const;
   Matrix& operator*=(const Matrix& rhs);
   

   Matrix operator+(float val) const;
   Matrix& operator+=(float val);
   Matrix operator-(float val) const;
   Matrix& operator-=(float val);
   Matrix operator*(float val) const;
   Matrix& operator*=(float val);
   Matrix operator/(float val) const;
   Matrix& operator/=(float val);

   operator D3DXMATRIX() const;
   operator EulerAngles() const;

   // -------------------------------------------------------------------------
   // Operations
   // -------------------------------------------------------------------------

   /**
    * Returns the matrix right vector.
    */
   Vector rightVec() const;

   /**
    * Returns the matrix up vector.
    */
   Vector upVec() const;

   /**
    * Returns the matrix look vector.
    */
   Vector lookVec() const;

   /**
    * Returns the matrix look vector.
    */
   Vector position() const;

   /**
    * Returns the scale the matrix describes.
    */
   Vector scale() const;

   /**
    * Rotates the specified vector.
    *
    * @param rhs     vector to be rotated.
    */
   Vector transform(const Vector& rhs) const;

   /**
    * Rotates the specified vector, but returns its normalized version
    *
    * @param rhs     vector to be rotated.
    */
   Vector transformNorm(const Vector& rhs) const;

   /**
    * This method writes the contents of the orientation to the specified 
    * output stream.
    *
    * @param stream     output stream
    * @param rhs        orientation description of which we want to output
    */
   friend std::ostream& operator<<(std::ostream& stream, const Matrix& rhs);
};

///////////////////////////////////////////////////////////////////////////////
