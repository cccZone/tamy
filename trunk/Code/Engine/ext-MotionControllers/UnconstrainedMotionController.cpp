#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

UnconstrainedMotionController::UnconstrainedMotionController( Node& controlledNode )
   : m_controlledNode( controlledNode )
{
}

///////////////////////////////////////////////////////////////////////////////

void UnconstrainedMotionController::move(const D3DXVECTOR3& transVec)
{
   D3DXVECTOR3 currPos = getPosition();
   setPosition(currPos + transVec);
}

///////////////////////////////////////////////////////////////////////////////

void UnconstrainedMotionController::rotate(float pitch, float yaw, float roll)
{
   D3DXVECTOR3 rightVec = m_controlledNode.getRightVec();
   D3DXVECTOR3 upVec = m_controlledNode.getUpVec();
   D3DXVECTOR3 lookVec = m_controlledNode.getLookVec();

   if (pitch)
   {
      D3DXMATRIX mtxRotate;
      D3DXMatrixIdentity(&mtxRotate);
      D3DXMatrixRotationAxis(&mtxRotate, &rightVec, D3DXToRadian(pitch));

      D3DXVec3TransformNormal(&upVec, &upVec, &mtxRotate);
      D3DXVec3TransformNormal(&lookVec, &lookVec, &mtxRotate);
   }

   if (yaw)
   {
      D3DXMATRIX mtxRotate;
      D3DXMatrixIdentity(&mtxRotate);
      D3DXMatrixRotationAxis(&mtxRotate, &upVec, D3DXToRadian(yaw));

      D3DXVec3TransformNormal(&rightVec, &rightVec, &mtxRotate);
      D3DXVec3TransformNormal(&lookVec, &lookVec, &mtxRotate);
   }

   if (roll)
   {
      D3DXMATRIX mtxRotate;
      D3DXMatrixIdentity(&mtxRotate);
      D3DXMatrixRotationAxis(&mtxRotate, &lookVec, D3DXToRadian(roll));

      D3DXVec3TransformNormal(&rightVec, &rightVec, &mtxRotate);
      D3DXVec3TransformNormal(&upVec, &upVec, &mtxRotate);
   }

   regenerateVectors(rightVec, upVec, lookVec);

   m_controlledNode.setRightVec(rightVec);
   m_controlledNode.setUpVec(upVec);
   m_controlledNode.setLookVec(lookVec);
}

///////////////////////////////////////////////////////////////////////////////

void UnconstrainedMotionController::rotate( float pitch, float yaw )
{
   D3DXVECTOR3 rightVec = m_controlledNode.getRightVec();
   D3DXVECTOR3 lookVec = m_controlledNode.getLookVec();
   static D3DXVECTOR3 EY( 0, 1, 0 );

   if ( pitch )
   {
      D3DXMATRIX mtxRotate;
      D3DXMatrixIdentity( &mtxRotate );
      D3DXMatrixRotationAxis( &mtxRotate, &rightVec, D3DXToRadian( pitch ) );

      D3DXVec3TransformNormal( &lookVec, &lookVec, &mtxRotate );
   }

   if ( yaw )
   {
      D3DXMATRIX mtxRotate;
      D3DXMatrixIdentity( &mtxRotate );
      D3DXMatrixRotationAxis( &mtxRotate, &EY, D3DXToRadian( yaw ) );

      D3DXVec3TransformNormal( &rightVec, &rightVec, &mtxRotate );
      D3DXVec3TransformNormal( &lookVec, &lookVec, &mtxRotate );
   }

   // regenerate vectors
   D3DXVECTOR3 upVec = EY;
   regenerateVectors( rightVec, upVec, lookVec );

   m_controlledNode.setRightVec( rightVec );
   m_controlledNode.setUpVec( upVec );
   m_controlledNode.setLookVec( lookVec );
}

///////////////////////////////////////////////////////////////////////////////

void UnconstrainedMotionController::setPosition( const D3DXVECTOR3& vec )
{
   D3DXVECTOR3 currentGlobalPos = getPosition();
   D3DXMATRIX newLocalMtx = m_controlledNode.getLocalMtx();

   newLocalMtx._41 += vec.x - currentGlobalPos.x;
   newLocalMtx._42 += vec.y - currentGlobalPos.y;
   newLocalMtx._43 += vec.z - currentGlobalPos.z;
   m_controlledNode.setLocalMtx(newLocalMtx);
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 UnconstrainedMotionController::getRightVec() const
{
   const D3DXMATRIX& globalMtx = m_controlledNode.getGlobalMtx();
   return D3DXVECTOR3(globalMtx._11, globalMtx._12, globalMtx._13);
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 UnconstrainedMotionController::getUpVec() const
{
   const D3DXMATRIX& globalMtx = m_controlledNode.getGlobalMtx();
   return D3DXVECTOR3(globalMtx._21, globalMtx._22, globalMtx._23);
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 UnconstrainedMotionController::getLookVec() const
{
   const D3DXMATRIX& globalMtx = m_controlledNode.getGlobalMtx();
   return D3DXVECTOR3(globalMtx._31, globalMtx._32, globalMtx._33);
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 UnconstrainedMotionController::getPosition() const
{
   const D3DXMATRIX& globalMtx = m_controlledNode.getGlobalMtx();
   return D3DXVECTOR3(globalMtx._41, globalMtx._42, globalMtx._43);
}

///////////////////////////////////////////////////////////////////////////////

void UnconstrainedMotionController::regenerateVectors( D3DXVECTOR3& rightVec, D3DXVECTOR3& upVec, D3DXVECTOR3& lookVec) const
{
   D3DXVec3Normalize( &lookVec, &lookVec );
   D3DXVec3Cross( &rightVec, &upVec, &lookVec );
   D3DXVec3Normalize( &rightVec, &rightVec );
   D3DXVec3Cross( &upVec, &lookVec, &rightVec );
   D3DXVec3Normalize( &upVec, &upVec );
}

///////////////////////////////////////////////////////////////////////////////
