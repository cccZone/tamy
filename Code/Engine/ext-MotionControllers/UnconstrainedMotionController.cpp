#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core\Node.h"
#include "core\Math.h"


///////////////////////////////////////////////////////////////////////////////

UnconstrainedMotionController::UnconstrainedMotionController( Node& controlledNode )
   : m_controlledNode( controlledNode )
{
}

///////////////////////////////////////////////////////////////////////////////

void UnconstrainedMotionController::move(const Vector& transVec)
{
   Vector currPos = getPosition();
   currPos.add( transVec );
   setPosition( currPos );
}

///////////////////////////////////////////////////////////////////////////////

void UnconstrainedMotionController::rotate( float pitch, float yaw, float roll )
{
   Vector rightVec, upVec, lookVec;
   m_controlledNode.getRightVec( rightVec );
   m_controlledNode.getUpVec( upVec );
   m_controlledNode.getLookVec( lookVec );

   if (pitch)
   {
      static FastFloat maxPosPitch = FastFloat::fromFloat( 89.0f );
      static FastFloat maxNegPitch = FastFloat::fromFloat( -89.0f );

      FastFloat simdPitch;
      simdPitch.setClamped( FastFloat::fromFloat( pitch ), maxNegPitch, maxPosPitch );
      simdPitch.mul( Float_Deg2Rad );

      Quaternion rotQ;
      rotQ.setAxisAngle( rightVec, simdPitch );
      
      Vector tmpVec;
      rotQ.transform( upVec, tmpVec ); upVec = tmpVec;
      rotQ.transform( lookVec, tmpVec ); lookVec = tmpVec;
   }

   if (yaw)
   {
      FastFloat simdYaw;
      simdYaw.setFromFloat( yaw );
      simdYaw.mul( Float_Deg2Rad );

      Quaternion rotQ;
      rotQ.setAxisAngle( upVec, simdYaw );

      Vector tmpVec;
      rotQ.transform( rightVec, tmpVec ); rightVec = tmpVec;
      rotQ.transform( lookVec, tmpVec ); lookVec = tmpVec;
   }

   if (roll)
   {
      FastFloat simdRoll;
      simdRoll.setFromFloat( roll );
      simdRoll.mul( Float_Deg2Rad );

      Quaternion rotQ;
      rotQ.setAxisAngle( lookVec, simdRoll );

      Vector tmpVec;
      rotQ.transform( rightVec, tmpVec ); rightVec = tmpVec;
      rotQ.transform( upVec, tmpVec ); upVec = tmpVec;
   }

   regenerateVectors( rightVec, upVec, lookVec );

   m_controlledNode.setRightVec(rightVec);
   m_controlledNode.setUpVec(upVec);
   m_controlledNode.setLookVec(lookVec);
}

///////////////////////////////////////////////////////////////////////////////

void UnconstrainedMotionController::rotate( float pitch, float yaw )
{
   Vector rightVec, lookVec;
   m_controlledNode.getRightVec( rightVec );
   m_controlledNode.getLookVec( lookVec );

   Quaternion currRotationQuat;
   m_controlledNode.getGlobalMtx().getRotation( currRotationQuat );

   if ( pitch )
   {
      Vector lookVec2D;   
      lookVec2D.setCross( rightVec, Quad_0100 );

      FastFloat currPitch = Float_0;
      {
         FastFloat currPitchDot = lookVec2D.dot( lookVec );
         currPitchDot.setClamped( currPitchDot, Float_Minus1, Float_1 );
         currPitchDot.acos();

         FastFloat dot;
         dot.setNeg( lookVec.dot( Quad_0100 ) );
         currPitch.setFlipSign( currPitchDot, dot );
         currPitch.mul( Float_Rad2Deg );
      }

      static FastFloat maxPosPitch = FastFloat::fromFloat( 89.0f );
      static FastFloat maxNegPitch = FastFloat::fromFloat( -89.0f );

      FastFloat newPitch;
      newPitch.setAdd( currPitch, FastFloat::fromFloat( pitch ) );
      newPitch.setClamped( newPitch, maxNegPitch, maxPosPitch );
      newPitch.mul( Float_Deg2Rad );

      Quaternion rotQ;
      rotQ.setAxisAngle( rightVec, newPitch );

      Vector tmpVec;
      rotQ.transform( lookVec2D, lookVec );
   }

   if ( yaw )
   {
      FastFloat simdYaw;
      simdYaw.setFromFloat( yaw );
      simdYaw.mul( Float_Deg2Rad );

      Quaternion rotQ;
      rotQ.setAxisAngle( Quad_0100, simdYaw );

      Vector tmpVec;
      rotQ.transform( rightVec, tmpVec ); rightVec = tmpVec;
      rotQ.transform( lookVec, tmpVec ); lookVec = tmpVec;
   }

   // regenerate vectors
   Vector upVec = Quad_0100;
   regenerateVectors( rightVec, upVec, lookVec );

   m_controlledNode.setRightVec( rightVec );
   m_controlledNode.setUpVec( upVec );
   m_controlledNode.setLookVec( lookVec );
}

///////////////////////////////////////////////////////////////////////////////

void UnconstrainedMotionController::setPosition( const Vector& vec )
{
   const Vector& currentGlobalPos = getPosition();
   Matrix newLocalMtx = m_controlledNode.getLocalMtx();

   Vector newPos;
   newPos.setSub( vec, currentGlobalPos );
   newPos.setAdd( newLocalMtx.position(), newPos );
   newLocalMtx.setPosition<3>( newPos );

   m_controlledNode.setLocalMtx(newLocalMtx);
}

///////////////////////////////////////////////////////////////////////////////

const Vector& UnconstrainedMotionController::getRightVec() const
{
   const Matrix& globalMtx = m_controlledNode.getGlobalMtx();
   return globalMtx.sideVec();
}

///////////////////////////////////////////////////////////////////////////////

const Vector& UnconstrainedMotionController::getUpVec() const
{
   const Matrix& globalMtx = m_controlledNode.getGlobalMtx();
   return globalMtx.upVec();
}

///////////////////////////////////////////////////////////////////////////////

const Vector& UnconstrainedMotionController::getLookVec() const
{
   const Matrix& globalMtx = m_controlledNode.getGlobalMtx();
   return globalMtx.forwardVec();
}

///////////////////////////////////////////////////////////////////////////////

const Vector& UnconstrainedMotionController::getPosition() const
{
   const Matrix& globalMtx = m_controlledNode.getGlobalMtx();
   return globalMtx.position();
}

///////////////////////////////////////////////////////////////////////////////

void UnconstrainedMotionController::regenerateVectors( Vector& rightVec, Vector& upVec, Vector& lookVec) const
{
   lookVec.normalize();
   rightVec.setCross( upVec, lookVec );
   rightVec.normalize();
   upVec.setCross( lookVec, rightVec );
   upVec.normalize();
}

///////////////////////////////////////////////////////////////////////////////
