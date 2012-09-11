/// @file   core\Matrix.h
/// @brief  matrix representation
#pragma once

#include <iostream>
#include "core/types.h"
#include "core/Vector.h"


///////////////////////////////////////////////////////////////////////////////

struct Vector;
struct EulerAngles;
struct Plane;
struct Quaternion;
class OutStream;
class InStream;

///////////////////////////////////////////////////////////////////////////////

/**
 * Mathematical matrix - expressed in row major order expressed in left-handed 
 * Cartesian coordinate system.
 */
struct Matrix
{
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
   Matrix( const float* values );

   /**
    * Constructor.
    *
    * @param 16 float values
    */
   Matrix( float _11, float _12, float _13, float _14, 
           float _21, float _22, float _23, float _24, 
           float _31, float _32, float _33, float _34,
           float _41, float _42, float _43, float _44 );


   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   Matrix& operator=(const Matrix& rhs);
   bool operator==(const Matrix& rhs) const;
   bool operator!=(const Matrix& rhs) const;

   // -------------------------------------------------------------------------
   // Operations
   // -------------------------------------------------------------------------

   /**
    * Creates a translation matrix with translation equal to the specified vector.
    *
    * @param translationVec
    */
   Matrix& setTranslation( const Vector& translationVec );

   /**
    * Creates a rotation matrix with rotation equal to the specified quaternion.
    *
    * @param rotationQuat
    */
   Matrix& setRotation( const Quaternion& rotationQuat );

   /**
    * Creates a rotation matrix with rotation corresponding to the specified Euler angle value
    *
    * @param angles
    */
   Matrix& setRotation( const EulerAngles& angles );

   /**
    * Returns the rotation this matrix describes in the form of a quaternion.
    *
    * @param outRotationQuat
    */
   void getRotation( Quaternion& outRotationQuat ) const;

   /**
    * Returns the rotation this matrix describes in the form of Euler angles.
    *
    * @param outAngles
    */
   void getRotation( EulerAngles& outAngles ) const;

   /**
    * this = a * b
    *
    * @param a
    * @param b
    */
   Matrix& setMul( const Matrix& a, const Matrix& b );

   /**
    * this = this * a
    *
    * CAUTION - it allocates temporary data on stack, thus it's slower than setMul.
    *
    * @param a
    */
   Matrix& mul( const Matrix& a );

   /**
    * this = a * this
    *
    * CAUTION - it allocates temporary data on stack, thus it's slower than setMul.
    *
    * @param a
    */
   Matrix& preMul( const Matrix& a );

   /**
    * this = a * t
    *
    * @param a
    * @param t
    */
   Matrix& setMul( const Matrix& a, float t );

   /**
    * this *= t
    *
    * @param t
    */
   Matrix& mul( float t );

   /**
    * Sets an inverse of the specified matrix.
    *
    * @param rhs
    */
   Matrix& setInverse( const Matrix& rhs );

   /**
    * Inverts the matrix.
    */
   Matrix& invert();

   /**
    * Sets a transposed version of the specified matrix.
    *
    * @param rhs
    */
   Matrix& setTransposed( const Matrix& rhs );

   /**
    * Transposes the matrix.
    */
   Matrix& transpose();

   /**
    * Sets an inverse transpose of the specified matrix.
    *
    *  this = ( rhs^-1 )^t
    *
    * @param rhs
    */
   Matrix& setInverseTranspose( const Matrix& rhs );

   /**
    * Returns the matrix side vector.
    */
   const Vector& sideVec() const;

   /**
    * Returns the matrix up vector.
    */
   const Vector& upVec() const;

   /**
    * Returns the matrix forward vector.
    */
   const Vector& forwardVec() const;

   /**
    * Returns the matrix position.
    */
   const Vector& position() const;

   /**
    * Returns the specified row vector.
    *
    * @param axisIdx    values in range <0..3> ONLY
    */
   const Vector& getRow( byte rowIdx ) const;

   /**
    * Sets the new value for the row that represents the side axis.
    *
    * @param vec
    */
   void setSideVec( const Vector& vec );

   /**
    * Sets the new value for the row that represents the up axis.
    *
    * @param vec
    */
   void setUpVec( const Vector& vec );

   /**
    * Sets the new value for the row that represents the forward axis.
    *
    * @param vec
    */
   void setForwardVec( const Vector& vec );

   /**
    * Sets the new value for the row that represents the position.
    *
    * @param pos
    */
   void setPosition( const Vector& pos );

   /**
    * Returns the coordinate system axes and the position vector.
    *
    * @param outSideVec
    * @param outUpVec
    * @param outForwardVec
    * @param outPos
    */
   void getVectors( Vector& outSideVec, Vector& outUpVec, Vector& outForwardVec, Vector& outPos ) const;

   /**
    * Composes the matrix from the specified row vectors
    *
    * @param sideVec
    * @param upVec
    * @param forwardVec
    * @param pos
    */
   void setRows( const Vector& sideVec, const Vector& upVec, const Vector& forwardVec, const Vector& pos = Vector::OW );

   /**
    * Returns the scale the matrix describes.
    *
    * @param outScaleVec
    */
   void getScale( Vector& outScaleVec ) const;

   /**
    * Applies a uniform scale to this matrix ( it doesn't override any coefficients, but is multiplied by them ).
    *
    * @param scale
    */
   void scaleUniform( float scale );

   /**
    * Applies a non-uniform scale to this matrix ( it doesn't override any coefficients, but is multiplied by them ).
    *
    * @param scaleVec
    */
   void scale( const Vector& scaleVec );


   /**
    * Transforms the specified vector.
    *
    * @param inVec     vector to be transformed.
    * @param outVec  transformed vector
    */
   void transform( const Vector& inVec, Vector& outVec ) const;

   /**
    * Transforms the specified normal vector
    *
    * @param inNorm        normal vector to be transformed.
    * @param outNorm    transformed normal
    */
   void transformNorm( const Vector& inNorm, Vector& outNorm ) const;

   /**
    * Transforms the specified plane.
    *
    * @param inPlane     plane to be transformed.
    * @param outPlane  transformed plane
    */
   void transform( const Plane& inPlane, Plane& outPlane ) const;

   /**
    * Transforms a vector taking its four elements into account, 
    * not homogenizing it back to 3 dimensions afterwards.
    *
    * @param rhs
    * @param outVec
    */
   void transform4( const Vector& rhs, Vector& outVec ) const;

   /**
    * This method writes the contents of the orientation to the specified 
    * output stream.
    *
    * @param stream     output stream
    * @param rhs        orientation description of which we want to output
    */
   friend std::ostream& operator<<(std::ostream& stream, const Matrix& rhs);

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend OutStream& operator<<( OutStream& serializer, const Matrix& rhs );
   friend InStream& operator>>( InStream& serializer, Matrix& rhs );

};

///////////////////////////////////////////////////////////////////////////////
