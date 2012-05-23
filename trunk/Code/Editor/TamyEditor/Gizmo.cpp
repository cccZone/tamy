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
   setMode( GM_TRANSLATION );
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
   switch( m_mode )
   {
   case GM_TRANSLATION:
      {
         drawTranslationGizmo( renderer );
         break;
      }

   case GM_ROTATION:
      {
         drawRotationGizmo( renderer );
         break;
      }

   case GM_SCALING:
      {
         drawScalingGizmo( renderer );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::drawTranslationGizmo( Renderer& renderer ) const
{
   const Matrix& mtx = m_node.getGlobalMtx();

   Vector orig, ox, oy, oz;
   mtx.transform( Vector::ZERO, orig );
   mtx.transform( Vector( SIZE, 0, 0 ), ox );
   mtx.transform( Vector( 0, SIZE, 0 ), oy );
   mtx.transform( Vector( 0, 0, SIZE ), oz );

   new ( renderer() ) RCDrawDebugLine( orig, ox, OX_COLOR ); // OX
   new ( renderer() ) RCDrawDebugLine( orig, oy, OY_COLOR ); // OY
   new ( renderer() ) RCDrawDebugLine( orig, oz, OZ_COLOR ); // OZ
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::drawRotationGizmo( Renderer& renderer ) const
{
   const Matrix& mtx = m_node.getGlobalMtx();

   Vector orig, ox, oy, oz;
   mtx.transform( Vector::ZERO, orig );
   mtx.transform( Vector( SIZE, 0, 0 ), ox );
   mtx.transform( Vector( 0, SIZE, 0 ), oy );
   mtx.transform( Vector( 0, 0, SIZE ), oz );

   // OX
   new ( renderer() ) RCDrawDebugLine( orig, ox, OX_COLOR );
   new ( renderer() ) RCDrawDebugArc( ox, oz, OX_COLOR );

   // OY
   new ( renderer() ) RCDrawDebugLine( orig, oy, OY_COLOR );
   new ( renderer() ) RCDrawDebugArc( oy, oz, OX_COLOR );


   // OZ
   new ( renderer() ) RCDrawDebugLine( orig, oz, OZ_COLOR );
   new ( renderer() ) RCDrawDebugArc( oz, ox, OX_COLOR );
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::drawScalingGizmo( Renderer& renderer ) const
{
   const Matrix& mtx = m_node.getGlobalMtx();

   Vector orig, ox, oy, oz;
   mtx.transform( Vector::ZERO, orig );
   mtx.transform( Vector( SIZE, 0, 0 ), ox );
   mtx.transform( Vector( 0, SIZE, 0 ), oy );
   mtx.transform( Vector( 0, 0, SIZE ), oz );


   const Vector boxSize( 0.2f, 0.2f, 0.2f );

   // OX
   Matrix boxMtx = mtx;
   boxMtx.m41 = ox.x; boxMtx.m42 = ox.y; boxMtx.m43 = ox.z;
   new ( renderer() ) RCDrawDebugLine( orig, ox, OX_COLOR );
   new ( renderer() ) RCDrawDebugBox( boxMtx, boxSize, OX_COLOR );

   // OY
   boxMtx.m41 = oy.x; boxMtx.m42 = oy.y; boxMtx.m43 = oy.z;
   new ( renderer() ) RCDrawDebugLine( orig, oy, OY_COLOR );
   new ( renderer() ) RCDrawDebugBox( boxMtx, boxSize, OY_COLOR );

   // OZ
   boxMtx.m41 = oz.x; boxMtx.m42 = oz.y; boxMtx.m43 = oz.z;
   new ( renderer() ) RCDrawDebugLine( orig, oz, OZ_COLOR );
   new ( renderer() ) RCDrawDebugBox( boxMtx, boxSize, OZ_COLOR );
}

///////////////////////////////////////////////////////////////////////////////
