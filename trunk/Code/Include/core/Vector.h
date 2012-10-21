/// @file   core\Vector.h
/// @brief  math vector
#ifndef _VECTOR_H
#define _VECTOR_H

#include "core/MemoryRouter.h"
#include "core/MathDataStorage.h"
#include "core/TVector.h"
#include "core/FastFloat.h"
#include <iostream>


///////////////////////////////////////////////////////////////////////////////

class OutStream;
class InStream;
struct FastFloat;

///////////////////////////////////////////////////////////////////////////////

/**
 * Result of a comparison of two vectors.
 */
#ifdef _USE_SIMD

   typedef __m128 VectorComparison;

#else
   struct VectorComparison
   {
      bool        b[4];
   };

#endif

///////////////////////////////////////////////////////////////////////////////

enum QuadMathConstants
{
   QuadMathConst_0,
   QuadMathConst_1,

   QuadMathConst_Minus1,

   QuadMathConst_1000,
   QuadMathConst_0100,
   QuadMathConst_0010,
   QuadMathConst_0001,
   QuadMathConst_Neg_1000,
   QuadMathConst_Neg_0100,
   QuadMathConst_Neg_0010,
   QuadMathConst_Neg_0001,

   QuadMathConst_MAX
};
extern QuadStorage          g_quadConstants[QuadMathConst_MAX];

///////////////////////////////////////////////////////////////////////////////

enum VecConstants
{
   VecConstants_OX,
   VecConstants_OY,
   VecConstants_OZ,
   VecConstants_OW,
   VecConstants_NEG_OX,
   VecConstants_NEG_OY,
   VecConstants_NEG_OZ,
   VecConstants_NEG_OW,
   VecConstants_ZERO,
   VecConstants_ONE,

   VecConstants_MAX
};
struct Vector;
extern Vector          q_vecConstants[VecConstants_MAX];

#define Vector_OX       q_vecConstants[ VecConstants_OX ]
#define Vector_OY       q_vecConstants[ VecConstants_OY ]
#define Vector_OZ       q_vecConstants[ VecConstants_OZ ]
#define Vector_OW       q_vecConstants[ VecConstants_OW ]
#define Vector_NEG_OX   q_vecConstants[ VecConstants_NEG_OX ]
#define Vector_NEG_OY   q_vecConstants[ VecConstants_NEG_OY ]
#define Vector_NEG_OZ   q_vecConstants[ VecConstants_NEG_OZ ]
#define Vector_NEG_OW   q_vecConstants[ VecConstants_NEG_OW ]
#define Vector_ZERO     q_vecConstants[ VecConstants_ZERO ]
#define Vector_ONE     q_vecConstants[ VecConstants_ONE ]

///////////////////////////////////////////////////////////////////////////////

/**
 * Mathematical vector (4d).
 */
struct Vector
{
   DECLARE_ALLOCATOR( Vector, AM_ALIGNED_16 );

   QuadStorage       m_quad;

   /**
    * Default constructor.
    */
   inline Vector();

   /**
    * @Obsolete
    *
    * Creates a vector from 4 float values.
    *
    * @param x, y, z, w    coordinates
    */
   inline explicit Vector( float x, float y, float z, float w = 0.0f );

   /**
    * Constructor.
    *
    * @param quad
    */
   inline Vector( const QuadStorage& quad );

   /**
    * Resets the vector to the default state.
    */
   inline void setZero();

   /**
    * Creates a vector from 4 float values
    *
    * @param x, y, z, w    coordinates
    */
   inline void set( float x, float y, float z, float w = 0.0f );

   /**
    * Creates a vector from an array of 4, 16-bytes aligned floats
    *
    * @param coords
    */
   inline void set( float* coords );

   /**
    * creates a vector from 4 fast float values
    *
    * @param x, y, z, w    coordinates
    */
   inline void set( const FastFloat& x, const FastFloat& y, const FastFloat& z, const FastFloat& w = Float_0 );

   /**
    * Creates a vector from a quad
    *
    * @param quad
    */
   inline void set( const QuadStorage& quad );

   /**
    *
    *  @param val
    * Sets all vector components to the specified value.
    */
   inline void setBroadcast( const FastFloat& val );

   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   /**
    * Compares four coordinates of the two vectors.
    */
   inline bool equals4( const Vector& rhs ) const;

   /**
    * Compares three coordinates of the two vectors.
    */
   inline bool operator==(const Vector& rhs) const;
   inline bool operator!=(const Vector& rhs) const;

   /**
    * Assignment operator.
    *
    * @param rhs
    */
   inline void operator=( const Vector& rhs );

   /**
    * Returns the specified vector component.
    *
    * @param idx
    */
   inline float& operator[]( int idx );
   inline float operator[]( int idx ) const;

   /**
    * Returns the specified vector component.
    *
    * @param idx
    */
   inline const FastFloat getComponent( uint idx ) const;

   // -------------------------------------------------------------------------
   // Operations
   // -------------------------------------------------------------------------

   /**
    * Sets the normalized value of the other vector.
    */
   inline void setNormalized( const Vector& vec );
   inline void setNormalized( float x, float y, float z );
   inline void setNormalized( const FastFloat& x, const FastFloat& y, const FastFloat& z );

   /**
    * Sets individual vector components depending on the specified vector comparison result.
    *
    * @param comparisonResult
    * @param trueVec             if the result for the component is true, a corresponding component from this vector will be selected
    * @param falseVec            opposite to the above
    */
   inline void setSelect( const VectorComparison& comparisonResult, const Vector& trueVec, const Vector& falseVec );

   /**
    * this = vec * t
    * 
    * @param vec
    * @param t
    */
   inline void setMul( const Vector& vec, const FastFloat& t );

   /**
    * this = vec1 * vec2 ( component by component )
    * 
    * @param vec1
    * @param vec2
    */
   inline void setMul( const Vector& vec1, const Vector& vec2 );

   /**
    * this = vec1 * t + vec2
    * 
    * @param vec1
    * @param t
    * @param vec2
    */
   inline void setMulAdd( const Vector& vec1, const FastFloat& t, const Vector& vec2 );

   /**
    * this = vec1 * t + vec2 ( component by component )
    * 
    * @param vec1
    * @param t
    * @param vec2
    */
   inline void setMulAdd( const Vector& vec1, const Vector& t, const Vector& vec2 );

   /**
    * this = vec1 / vec2 ( component by component )
    * 
    * @param vec1
    * @param vec2
    */
   inline void setDiv( const Vector& vec1, const Vector& vec2 );

   /**
    * this = vec1 + vec2
    * 
    * @param vec1
    * @param vec2
    */
   inline void setAdd( const Vector& vec1, const Vector& vec2 );

   /**
    * this = vec1 - vec2
    * 
    * @param vec1
    * @param vec2
    */
   inline void setSub( const Vector& vec1, const Vector& vec2 );

   /**
    * this = this * t
    * 
    * @param t
    */
   inline void mul( const FastFloat& t );

   /**
    * this = this * vec ( component by component )
    * 
    * @param vec
    */
   inline void mul( const Vector& vec );

   /**
    * this = this / vec ( component by component )
    * 
    * @param vec
    */
   inline void div( const Vector& vec );

   /**
    * this = this  + vec
    * 
    * @param vec
    */
   inline void add( const Vector& vec );

   /**
    * this = this - vec
    * 
    * @param vec
    */
   inline void sub( const Vector& vec );

   /**
    * this = this * -1
    */
   inline void neg();

   /**
    * Normalizes this vector (in place).
    */
   inline void normalize();

   /**
    * Calculates the floor of each of the vector's components.
    */
   inline void floor();

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
   inline void setLerp( const Vector& a, const Vector& b, const FastFloat& t );

   /**
    * Calculate a dot product with another vector ( taking only 3 coordinates into account ).
    *
    * @param rhs     other vector we want to use in the calculations
    */
   inline const FastFloat dot( const Vector& rhs ) const;

   /**
    * Calculate a dot product with another vector ( taking all 4 coordinates into account ).
    *
    * @param rhs     other vector we want to use in the calculations
    */
   inline const FastFloat dot4( const Vector& rhs ) const;

   /**
    * Creates a vector that's a result of a cross product between the specified vectors.
    *
    * @param v1
    * @param v2
    */
   inline void setCross( const Vector& v1, const Vector& v2 );

   /**
    * Calculates a cross product between this and the specified vector and stores
    * the results in this vector. 
    * WARNING: It creates a temporary vector, so might be slower than setCross method.
    *
    * this = this x rhs
    *
    * @param rhs
    */
   inline void preCross( const Vector& rhs );

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
   inline void postCross( const Vector& rhs );

   /**
    * Returns the length of this vector.
    */
   inline const FastFloat length() const;

   /**
    * Returns the squared length of this vector.
    */
   inline const FastFloat lengthSq() const;

   /**
    * Checks if the vector is unit-length.
    */
   bool isNormalized() const;

   /**
    * Returns a component of the lowest value.
    *
    * @param N    how many components should be taken into account during the comparison
    */
   template< int N >
   const FastFloat getMin() const;

   /**
    * Returns a component of the highest value.
    *
    * @param N    how many components should be taken into account during the comparison
    */
   template< int N >
   const FastFloat getMax() const;

   /**
    * this = ( a > b ) ? a : b )
    *
    * @param a
    * @param b
    */ 
   inline void setMax( const Vector& a, const Vector& b );

   /**
    * this = ( a < b ) ? a : b )
    *
    * @param a
    * @param b
    */ 
   inline void setMin( const Vector& a, const Vector& b );

   /**
    * this = min( minVal, max( a, maxVal ) )
    *
    * @param a
    * @param b
    */ 
   inline void setClamped( const Vector& a, const Vector& minVal, const Vector& maxVal );

   /**
    * Stores the contents of this vector in the specified raw vector instance.
    *
    * @param rawVector
    */
   template< int Dim >
   void store( TVector< Dim >& rawVector );

   /**
    * Loads the contents of the specified raw vector instance and initializes this vector with it.
    *
    * @param rawVector
    */
   template< int Dim >
   void load( const TVector< Dim >& rawVector );

   /**
    * Stores the contents of this vector in the specified quad storage.
    *
    * @param storage
    */
   inline void store( QuadStorage& storage ) const;

   /**
    * Initializes the vector with the contents of the specified quad storage.
    *
    * @param storage
    */
   inline void load( const QuadStorage& storage );

   // -------------------------------------------------------------------------
   // Vector comparison
   // -------------------------------------------------------------------------

   /**
    * Compares two vectors component by component: this < rhs
    *
    * @param rhs
    * @param outResult
    */
   inline void less( const Vector& rhs, VectorComparison& outResult ) const;

   /**
    * Compares two vectors component by component: this <= rhs
    *
    * @param rhs
    * @param outResult
    */
   inline void lessEqual( const Vector& rhs, VectorComparison& outResult ) const;

   /**
    * Compares two vectors component by component: this > rhs
    *
    * @param rhs
    * @param outResult
    */
   inline void greater( const Vector& rhs, VectorComparison& outResult ) const;

   /**
    * Compares two vectors component by component: this >= rhs
    *
    * @param rhs
    * @param outResult
    */
   inline void greaterEqual( const Vector& rhs, VectorComparison& outResult ) const;

   /**
    * Compares two vectors component by component: this == rhs
    *
    * @param rhs
    * @param outResult
    */
   inline void equal( const Vector& rhs, VectorComparison& outResult ) const;

   /**
    * Compares two vectors component by component: this != rhs
    *
    * @param rhs
    * @param outResult
    */
   inline void notEqual( const Vector& rhs, VectorComparison& outResult ) const;

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend std::ostream& operator<<(std::ostream& stream, const Vector& rhs);
   friend OutStream& operator<<( OutStream& serializer, const Vector& rhs );
   friend InStream& operator>>( InStream& serializer, Vector& rhs );
};

///////////////////////////////////////////////////////////////////////////////

#ifdef _USE_SIMD
   #include "core/VectorSimd.inl"
#else
   #include "core/VectorFpu.inl"
#endif

///////////////////////////////////////////////////////////////////////////////

#endif // _VECTOR_H
