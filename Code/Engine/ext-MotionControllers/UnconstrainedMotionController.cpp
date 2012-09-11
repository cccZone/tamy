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

void UnconstrainedMotionController::rotate(float pitch, float yaw, float roll)
{
   Vector rightVec, upVec, lookVec;
   m_controlledNode.getRightVec( rightVec );
   m_controlledNode.getUpVec( upVec );
   m_controlledNode.getLookVec( lookVec );

   if (pitch)
   {
      pitch = clamp( pitch, -89.0f, 89.0f );
      Quaternion rotQ;
      rotQ.setAxisAngle( rightVec, DEG2RAD( pitch ) );
      
      Vector tmpVec;
      rotQ.transform( upVec, tmpVec ); upVec = tmpVec;
      rotQ.transform( lookVec, tmpVec ); lookVec = tmpVec;
   }

   if (yaw)
   {
      Quaternion rotQ;
      rotQ.setAxisAngle( upVec, DEG2RAD( yaw ) );

      Vector tmpVec;
      rotQ.transform( rightVec, tmpVec ); rightVec = tmpVec;
      rotQ.transform( lookVec, tmpVec ); lookVec = tmpVec;
   }

   if (roll)
   {
      Quaternion rotQ;
      rotQ.setAxisAngle( lookVec, DEG2RAD( roll ) );

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
      lookVec2D.setCross( rightVec, Vector::OY );

      float currPitch = 0.0f;
      {
         float currPitchDot = lookVec2D.dot( lookVec );
         currPitchDot = clamp( currPitchDot, -1.0f, 1.0f );
         float currPitchSign = lookVec.dot( Vector::OY ) > 0 ? -1.0f : 1.0f;

         currPitch = RAD2DEG( currPitchSign * acos( currPitchDot ) );
      }

      float newPitch = currPitch + pitch;
      newPitch = clamp( newPitch, -89.0f, 89.0f );

      Quaternion rotQ;
      rotQ.setAxisAngle( rightVec, DEG2RAD( newPitch ) );

      Vector tmpVec;
      rotQ.transform( lookVec2D, lookVec );
   }

   if ( yaw )
   {
      Quaternion rotQ;
      rotQ.setAxisAngle( Vector::OY, DEG2RAD( yaw ) );

      Vector tmpVec;
      rotQ.transform( rightVec, tmpVec ); rightVec = tmpVec;
      rotQ.transform( lookVec, tmpVec ); lookVec = tmpVec;
   }

   // regenerate vectors
   Vector upVec = Vector::OY;
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

   newLocalMtx.m41 += vec.x - currentGlobalPos.x;
   newLocalMtx.m42 += vec.y - currentGlobalPos.y;
   newLocalMtx.m43 += vec.z - currentGlobalPos.z;
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
