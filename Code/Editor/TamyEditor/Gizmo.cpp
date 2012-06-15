#include "Gizmo.h"
#include "core-Renderer.h"
#include "core.h"
#include "core-MVC.h"


///////////////////////////////////////////////////////////////////////////////

Gizmo::Gizmo( SpatialEntity& node )
   : m_node( node )
   , SIZE( 10.f )
   , OX_COLOR( 1, 0, 0, 1 )
   , OY_COLOR( 0, 1, 0, 1 )
   , OZ_COLOR( 0, 0, 1, 1 )
{
   // set the initial mode
   setMode( GM_SCALING );
}

///////////////////////////////////////////////////////////////////////////////

Gizmo::~Gizmo()
{
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::setMode( Mode mode )
{
   // set the new mode and the related geometry
   m_mode = mode;
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::onDebugRender( Renderer& renderer ) const
{
   // we want the gizmo's size to be always the same - no matter how far away the camera is from the object.
   // So calculate the distance to the object and rescale the gizmo accordingly
   const Matrix& nodeMtx = m_node.getGlobalMtx();

   Vector cameraPos;
   renderer.getActiveCamera().getPosition( cameraPos );
   cameraPos.sub( nodeMtx.position() );
   float distFromCamera = cameraPos.length();
   float size = SIZE * distFromCamera;

   // draw a proper gizmo shape
   switch( m_mode )
   {
   case GM_TRANSLATION:
      {
         drawTranslationGizmo( renderer, size );
         break;
      }

   case GM_ROTATION:
      {
         drawRotationGizmo( renderer, size );
         break;
      }

   case GM_SCALING:
      {
         drawScalingGizmo( renderer, size );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::drawTranslationGizmo( Renderer& renderer, float size ) const
{
   const Matrix& mtx = m_node.getGlobalMtx();
   const Vector& start = mtx.position();

   float armHalfLength = 0.01f * size;
   float armHalfDim = 0.0003f * size;

   Vector ox, oy, oz;
   mtx.transformNorm( Vector::OX, ox );
   mtx.transformNorm( Vector::OY, oy );
   mtx.transformNorm( Vector::OZ, oz );
   ox.mul( armHalfLength ).add( start );
   oy.mul( armHalfLength ).add( start );
   oz.mul( armHalfLength ).add( start );

   // OX
   new ( renderer() ) RCDrawDebugArrow( armHalfDim, start, ox, OX_COLOR, OVERLAY );

   // OY
   new ( renderer() ) RCDrawDebugArrow( armHalfDim, start, oy, OY_COLOR, OVERLAY );

   // OZ
   new ( renderer() ) RCDrawDebugArrow( armHalfDim, start, oz, OZ_COLOR, OVERLAY );
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::drawRotationGizmo( Renderer& renderer, float size ) const
{
   const Matrix& mtx = m_node.getGlobalMtx();
   const Vector& start = mtx.position();
   Quaternion rotQ;

   float ringRadius = 0.02f * size;
   float ringCircumferenceWidth = 0.0003f * size;

   Matrix transformOX, transformOY, transformOZ;
   Quaternion q; 

   // <math.todo> next candidate for optimization as soon as I introduce the Transform class - get rid of those matrices then!!!

   // calculate transform OX
   {
      q.setAxisAngle( mtx.sideVec(), DEG2RAD( 90.0f ) );
      mtx.getRotation( rotQ );
      rotQ.mul( q );

      transformOX.setRotation( rotQ );
      transformOX.setPosition( start );
   }

   // calculate transform OY
   {
      q.setAxisAngle( mtx.upVec(), DEG2RAD( 90.0f ) );
      mtx.getRotation( rotQ );
      rotQ.mul( q );

      transformOY.setRotation( rotQ );
      transformOY.setPosition( start );
   }

   // calculate transform OZ
   {
      q.setAxisAngle( mtx.forwardVec(), DEG2RAD( 90.0f ) );
      mtx.getRotation( rotQ );
      rotQ.mul( q );

      transformOZ.setRotation( rotQ );
      transformOZ.setPosition( start );
   }

   // OX
   new ( renderer() ) RCDrawDebugRing( ringCircumferenceWidth, transformOX, ringRadius, OX_COLOR, OVERLAY );
   
   // OY
   new ( renderer() ) RCDrawDebugRing( ringCircumferenceWidth, transformOY, ringRadius, OY_COLOR, OVERLAY );

   // OZ
   new ( renderer() ) RCDrawDebugRing( ringCircumferenceWidth, transformOZ, ringRadius, OZ_COLOR, OVERLAY );
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::drawScalingGizmo( Renderer& renderer, float size ) const
{
   const Matrix& mtx = m_node.getGlobalMtx();


   float armHalfLength = 0.01f * size;
   float armHalfDim = 0.0003f * size;

   Vector endBoxSize( armHalfDim, armHalfDim, armHalfDim );
   endBoxSize.mul( 6 );

   const Vector oxBoxSize( armHalfLength, armHalfDim, armHalfDim );
   const Vector oyBoxSize( armHalfDim, armHalfLength, armHalfDim );
   const Vector ozBoxSize( armHalfDim, armHalfDim, armHalfLength );

   // OX
   Matrix boxTransform = mtx;
   boxTransform.m41 += armHalfLength;
   new ( renderer() ) RCDrawDebugBox( boxTransform, oxBoxSize, OX_COLOR, OVERLAY );
   boxTransform.m41 += armHalfLength;
   new ( renderer() ) RCDrawDebugBox( boxTransform, endBoxSize, OX_COLOR, OVERLAY );

   // OY
   boxTransform.m41 = mtx.m41;
   boxTransform.m42 += armHalfLength;
   new ( renderer() ) RCDrawDebugBox( boxTransform, oyBoxSize, OY_COLOR, OVERLAY );
   boxTransform.m42 += armHalfLength;
   new ( renderer() ) RCDrawDebugBox( boxTransform, endBoxSize, OY_COLOR, OVERLAY );

   // OZ
   boxTransform.m42 = mtx.m42;
   boxTransform.m43 += armHalfLength;
   new ( renderer() ) RCDrawDebugBox( boxTransform, ozBoxSize, OZ_COLOR, OVERLAY );
   boxTransform.m43 += armHalfLength;
   new ( renderer() ) RCDrawDebugBox( boxTransform, endBoxSize, OZ_COLOR, OVERLAY );
}

///////////////////////////////////////////////////////////////////////////////
