#include "ml-Blender/TamyScene.h"


///////////////////////////////////////////////////////////////////////////////

void TamyMatrix::applyTo( Matrix& outTransform ) const
{
   Matrix tamyRotMtx;
   rotationMtx.store( tamyRotMtx );

   Quaternion rotQuat;
   tamyRotMtx.getRotation( rotQuat );

   float tmp = rotQuat[1];
   rotQuat[1] = -rotQuat[2];
   rotQuat[2] = tmp;

   // position coordinates for the geometry are swapped as well - not the case with the lights though!!!
   Vector swappedPos;
   position.store( swappedPos );
   swappedPos[3] = 0;
   
   outTransform.setRotation( rotQuat );
   outTransform.setPosition<3>( swappedPos );
}

///////////////////////////////////////////////////////////////////////////////

void TamyMatrix::applyToLight( Matrix& outTransform ) const
{
   Matrix tamyRotMtx;
   rotationMtx.store( tamyRotMtx );

   Vector side, up, forward, pos;
   tamyRotMtx.getVectors( side, up, forward, pos );
   up.mul( Float_Minus1 );
   forward.mul( Float_Minus1 );

   Vector tamyPos;
   position.store( tamyPos );

   outTransform.setRows( side, up, forward, tamyPos );
}

///////////////////////////////////////////////////////////////////////////////
