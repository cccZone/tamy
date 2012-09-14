#include "ml-Blender/TamyScene.h"


///////////////////////////////////////////////////////////////////////////////

void TamyMatrix::applyTo( Matrix& outTransform ) const
{
   Quaternion rotQuat;
   rotationMtx.getRotation( rotQuat );

   float tmp = rotQuat.q[1];
   rotQuat.q[1] = -rotQuat.q[2];
   rotQuat.q[2] = tmp;

   // position coordinates for the geometry are swapped as well - not the case with the lights though!!!
   Vector swappedPos;
   swappedPos.set( position.x, position.z, position.y, 0 );
   
   outTransform.setRotation( rotQuat );
   outTransform.setPosition( swappedPos );
}

///////////////////////////////////////////////////////////////////////////////

void TamyMatrix::applyToLight( Matrix& outTransform ) const
{
   Vector side, up, forward, pos;
   rotationMtx.getVectors( side, up, forward, pos );
   up.mul( -1.0f );
   forward.mul( -1.0f );

   outTransform.setRows( side, up, forward, position );
}

///////////////////////////////////////////////////////////////////////////////
