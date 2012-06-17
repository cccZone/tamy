/// @file   core\Vector.h
/// @brief  math vector
#ifndef _VECTOR_H
#define _VECTOR_H

#include <iostream>


///////////////////////////////////////////////////////////////////////////////

class OutStream;
class InStream;

///////////////////////////////////////////////////////////////////////////////

/**
 * A vector that can have an arbitrary, but fixed number of coordinates.
 *
 * It's not a mathematical vector though - it's just a data structure.
 * That's why it should not have any mathematical operations defined!!!
 * Instead, create a class that contains it and contains relevant math functions
 */
template< int Dim >
struct TVector
{
private:
   enum { ARR_SIZE = Dim };

public:

   float v[ARR_SIZE];

   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   float& operator[]( int idx );
   float operator[]( int idx ) const;

   // -------------------------------------------------------------------------
   // Operations
   // -------------------------------------------------------------------------

   /**
    * Sets a single value for all coordinates.
    */
   void setUniform( float val );

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   template< int VDim > friend std::ostream& operator<<( std::ostream& stream, const TVector< VDim >& rhs );
   template< int VDim > friend OutStream& operator<<( OutStream& serializer, const TVector< VDim >& rhs );
   template< int VDim > friend InStream& operator>>( InStream& serializer, TVector< VDim >& rhs );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Mathematical vector (4d).
 */
struct Vector
{
   union
   {
      TVector< 4 > v;

      struct 
      {
         float x, y, z, w;
      };
   };

   // -------------------------------------------------------------------------
   // Static values
   // -------------------------------------------------------------------------
   static Vector     ZERO;
   static Vector     ONE;
   static Vector     OX;
   static Vector     OY;
   static Vector     OZ;
   static Vector     OW;
   static Vector     OX_NEG;
   static Vector     OY_NEG;
   static Vector     OZ_NEG;
   static Vector     OW_NEG;


   /**
    * Default constructor.
    */
   Vector();

   /**
    * Constructor.
    *
    * @param x, y, z, w    coordinates
    */
   Vector( float x, float y, float z, float w = 0.0f );

   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   /**
    * Compares four coordinates of the two vectors.
    */
   bool equals4( const Vector& rhs ) const;

   /**
    * Compares three coordinates of the two vectors.
    */
   bool operator==(const Vector& rhs) const;
   bool operator!=(const Vector& rhs) const;
   inline float& operator[]( int idx );
   inline float operator[]( int idx ) const;

   Vector& operator=(const Vector& rhs);

   // -------------------------------------------------------------------------
   // Operations
   // -------------------------------------------------------------------------

   /**
    * Sets new coordinates on the vector.
    */
   Vector& set( float x, float y, float z, float w = 0.0f );

   /**
    * Sets the normalized value of the other vector.
    */
   Vector& setNormalized( const Vector& vec );
   Vector& setNormalized( float x, float y, float z );

   /**
    * this = vec * t
    * 
    * @param vec
    * @param t
    */
   Vector& setMul( const Vector& vec, float t );

   /**
    * this = vec1 * t + vec2
    * 
    * @param vec1
    * @param t
    * @param vec2
    */
   Vector& setMulAdd( const Vector& vec1, float t, const Vector& vec2 );

   /**
    * this = vec1 + vec2
    * 
    * @param vec1
    * @param vec2
    */
   Vector& setAdd( const Vector& vec1, const Vector& vec2 );

   /**
    * this = vec1 - vec2
    * 
    * @param vec1
    * @param vec2
    */
   Vector& setSub( const Vector& vec1, const Vector& vec2 );

   /**
    * this = this * t
    * 
    * @param t
    */
   Vector& mul( float t );

   /**
    * this = this  + vec
    * 
    * @param vec
    */
   Vector& add( const Vector& vec );

   /**
    * this = this - vec
    * 
    * @param vec
    */
   Vector& sub( const Vector& vec );

   /**
    * this = this * -1
    */
   Vector& neg();

   /**
    * Normalizes this vector (in place).
    */
   Vector& normalize();

   /**
    * Linearly interpolates a vector between values a and b ( from a to b )
    * at the distance t.
    *
    * this = a*(1-t) + b*t
    *
    * @param a
    * @param b
    * @param t
    */
   Vector& setLerp( const Vector& a, const Vector& b, float t );

   /**
    * Calculate a dot product with another vector ( taking only 3 coordinates into account ).
    *
    * @param rhs     other vector we want to use in the calculations
    */
   float dot( const Vector& rhs ) const;

   /**
    * Calculate a dot product with another vector ( taking all 4 coordinates into account ).
    *
    * @param rhs     other vector we want to use in the calculations
    */
   float dot4( const Vector& rhs ) const;

   /**
    * Creates a vector that's a result of a cross product between the specified vectors.
    *
    * @param v1
    * @param v2
    */
   Vector& setCross( const Vector& v1, const Vector& v2 );

   /**
    * Calculates a cross product between this and the specified vector and stores
    * the results in this vector. 
    * WARNING: It creates a temporary vector, so might be slower than setCross method.
    *
    * this = this x rhs
    *
    * @param rhs
    */
   Vector& preCross( const Vector& rhs );

   /**
    * Calculates a cross product between this and the specified vector and stores
    * the results in this vector. 
    * As opposed to the 'preCross' method, this calculates the final vector by crossing
    * the components in a different order.
    * WARNING: It creates a temporary vector, so might be slower than setCross method.
    *
    * this = rhs x this
    *
    * @param rhs
    */
   Vector& postCross( const Vector& rhs );

   /**
    * Returns the length of this vector.
    */
   float length() const;

   /**
    * Returns the squared length of this vector.
    */
   float lengthSq() const;

   /**
    * Checks if the vector is unit-length.
    */
   bool isNormalized() const;

   /**
    * Stores the contents of this vector in the specified raw vector instance.
    *
    * @param rawVector
    */
   template< int Dim >
   void store( TVector< Dim >& rawVector );

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend std::ostream& operator<<(std::ostream& stream, const Vector& rhs);
   friend OutStream& operator<<( OutStream& serializer, const Vector& rhs );
   friend InStream& operator>>( InStream& serializer, Vector& rhs );
};

///////////////////////////////////////////////////////////////////////////////

#include "core/Vector.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _VECTOR_H
