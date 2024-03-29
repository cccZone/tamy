#include "core.h"
#include "core/Transform.h"
#include "core/Matrix.h"
#include "core/EulerAngles.h"
#include "core/Plane.h"


///////////////////////////////////////////////////////////////////////////////

Transform Transform::IDENTITY;

///////////////////////////////////////////////////////////////////////////////

Transform::Transform()
{
   m_translation.setZero();
   m_rotation.setIdentity();
}

///////////////////////////////////////////////////////////////////////////////

Transform::Transform( const Quaternion& rotation )
   : m_rotation( rotation )
{
   m_translation.setZero();
}

///////////////////////////////////////////////////////////////////////////////

Transform::Transform( const Vector& translation )
   : m_translation( translation )
{
   m_rotation.setIdentity();
}

///////////////////////////////////////////////////////////////////////////////

Transform::Transform( const Vector& translation, const Quaternion& rotation )
   : m_translation( translation )
   , m_rotation( rotation )
{
}

///////////////////////////////////////////////////////////////////////////////

Transform& Transform::operator=( const Transform& rhs )
{
   m_translation = rhs.m_translation;
   m_rotation = rhs.m_rotation;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

bool Transform::operator==( const Transform& rhs ) const
{
   return m_translation == rhs.m_translation && m_rotation == rhs.m_rotation;
}

///////////////////////////////////////////////////////////////////////////////

bool Transform::operator!=( const Transform& rhs ) const
{
   return !( *this == rhs );
}

///////////////////////////////////////////////////////////////////////////////

Transform& Transform::setRotation( const EulerAngles& angles )
{
   m_rotation.setFromEulerAngles( angles );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

void Transform::getRotation( EulerAngles& outAngles ) const
{
   outAngles.setFromQuaternion( m_rotation );
}

///////////////////////////////////////////////////////////////////////////////

void Transform::toMatrix( Matrix& outMatrix ) const
{
   outMatrix.setRotation( m_rotation );
   outMatrix.setPosition<3>( m_translation );
}

///////////////////////////////////////////////////////////////////////////////

Transform& Transform::set( const Matrix& matrix )
{
   m_translation = matrix.position();
   matrix.getRotation( m_rotation );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Transform& Transform::setMul( const Transform& a, const Transform& b )
{
   b.m_rotation.transform( a.m_translation, m_translation );
   m_translation.add( b.m_translation );

   m_rotation.setMul( a.m_rotation, b.m_rotation );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Transform& Transform::mul( const Transform& a )
{
   Vector tmpTranslation;
   a.m_rotation.transform( m_translation, tmpTranslation );
   m_translation.setAdd( a.m_translation, tmpTranslation );

   m_rotation.mul( a.m_rotation );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Transform& Transform::preMul( const Transform& a )
{
   Vector tmpTranslation;
   m_rotation.transform( a.m_translation, tmpTranslation );
   m_translation.add( tmpTranslation );

   Quaternion newRot;
   newRot.setMul( a.m_rotation, m_rotation );
   m_rotation = newRot;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Transform& Transform::setInverse( const Transform& rhs )
{
   m_rotation.setConjugated( rhs.m_rotation );
   m_translation = rhs.m_translation;
   m_translation.neg();

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Transform& Transform::invert()
{
   m_rotation.conjugate();
   m_translation.neg();

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

void Transform::transform( const Vector& inVec, Vector& outVec ) const
{
   m_rotation.transform( inVec, outVec );
   outVec.add( m_translation );
}

///////////////////////////////////////////////////////////////////////////////

void Transform::transformNorm( const Vector& inNorm, Vector& outNorm ) const
{
   m_rotation.transform( inNorm, outNorm );
}

///////////////////////////////////////////////////////////////////////////////

void Transform::transform( const Plane& inPlane, Plane& outPlane ) const
{
   Vector planeNormal;
   inPlane.getNormal( planeNormal );

   Vector rotatedPlaneNormal;
   m_rotation.transform( planeNormal, rotatedPlaneNormal );

   Vector newDistVec;
   newDistVec[3] = rotatedPlaneNormal.dot( m_translation ).getFloat();
   rotatedPlaneNormal[3] = inPlane[3];
   
   Vector newPlaneEq;
   newPlaneEq.setSub( rotatedPlaneNormal, newDistVec );
   outPlane.set( newPlaneEq );
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& stream, const Transform& rhs )
{
   stream << "Translation: " << rhs.m_translation << " Rotation: " << rhs.m_rotation;
   return stream;
}

///////////////////////////////////////////////////////////////////////////////

OutStream& operator<<( OutStream& serializer, const Transform& rhs )
{
   serializer << rhs.m_translation;
   serializer << rhs.m_rotation;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

InStream& operator>>( InStream& serializer, Transform& rhs )
{
   serializer >> rhs.m_translation;
   serializer >> rhs.m_rotation;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////
