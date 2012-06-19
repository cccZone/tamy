/// @file   core/PLane.h
/// @brief  mathematical plane representation
#pragma once

#include <iostream>


///////////////////////////////////////////////////////////////////////////////

class OutStream;
class InStream;
struct Vector;

///////////////////////////////////////////////////////////////////////////////

/**
 * Mathematical plane representation.
 */
struct Plane
{
   union
   {
      float p[4];

      struct 
      {
         float a, b, c, d;
      };
   };

   /**
    * Default constructor.
    */
   Plane();

   /**
    * Constructor.
    *
    * @param a, b, c, d    plane parameters
    */
   Plane( float a, float b, float c, float d );


   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   Plane& operator=( const Plane& rhs );
   bool operator==( const Plane& rhs ) const;
   bool operator!=( const Plane& rhs ) const;

   // -------------------------------------------------------------------------
   // Operations
   // -------------------------------------------------------------------------

   /**
    * Sets the new components on the plane.
    *
    * @param a
    * @param b
    * @param c
    * @param d
    */
   Plane& set( float a, float b, float c, float d );

   /**
    * Determines a plane based on a point that lies on the plane and the normal of the plane.
    *
    * @param point
    * @param normal
    */
   Plane& setFromPointNormal( const Vector& point, const Vector& normal );

   /**
    * Determines a plane based on three specified coplanar points.
    *
    * @param p1
    * @param p2
    * @param p3
    */
   Plane& setFromPoints( const Vector& p1, const Vector& p2, const Vector& p3 );

   /**
    * Normalizes the plane
    */
   Plane& normalize();

   /**
    * Calculates a dot product treating the specified vector as a coordinate in space.
    * Perfect for calculating a distance from the plane to a point in space.
    *
    * @param coord
    */
   float dotCoord( const Vector& coord ) const;

   /**
    * Calculates a dot product treating the specified vector as a normal vector ( unit length vector ).
    *
    * @param normal
    */
   float dotNormal( const Vector& normal ) const;

   /**
    * Returns the normal of the plane.
    */
   void getNormal( Vector& outPlaneNormal ) const;

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend std::ostream& operator<<( std::ostream& stream, const Plane& rhs );
   friend OutStream& operator<<( OutStream& serializer, const Plane& rhs );
   friend InStream& operator>>( InStream& serializer, Plane& rhs );
};

///////////////////////////////////////////////////////////////////////////////
