#include "Gizmo.h"
#include "core-Renderer.h"
#include "core.h"
#include "GizmoEffect.h"

#define _USE_MATH_DEFINES
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

Gizmo::Gizmo( SpatialEntity& node, ResourcesManager& rm, Camera& camera )
: m_node( node )
, m_geometry( NULL )
, m_renderable( NULL )
{
   const float SIZE = 10.f;
   const Color oxColor( 1, 0, 0, 1 );
   const Color oyColor( 0, 1, 0, 1 );
   const Color ozColor( 0, 0, 1, 1 );

   // prepare the geometry of translation axes
   m_translationAxes = dynamic_cast< LineSegments*>( rm.findResource( "TranslationAxes" ) );
   if ( !m_translationAxes )
   {
      m_translationAxes = new LineSegments( "TranslationAxes" );
      rm.addResource( m_translationAxes );
      m_translationAxes->add( LineSegment( D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( SIZE, 0, 0 ), oxColor ) );  // OX
      m_translationAxes->add( LineSegment( D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, SIZE, 0 ), oyColor ) );  // OY
      m_translationAxes->add( LineSegment( D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, 0, SIZE ), ozColor ) );  // OZ
      m_translationAxes->rebuild();
   }

   // prepare the geometry of rotation circles
   m_rotationAxes = dynamic_cast< LineSegments*>( rm.findResource( "RotationAxes" ) );
   if ( !m_rotationAxes )
   {
      m_rotationAxes = new LineSegments( "RotationAxes" );
      rm.addResource( m_rotationAxes );

      std::vector< D3DXVECTOR3 > points;
      const float dAng = M_PI / 32.f;

      // OX
      m_rotationAxes->add( LineSegment( D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( SIZE, 0, 0 ), oxColor ) );
      points.push_back( D3DXVECTOR3( 0, SIZE, 0 ) );
      for ( unsigned int i = 0; i < 17; ++i )
      {
         points.push_back( D3DXVECTOR3( 0, SIZE * cos( dAng * (float)i ), SIZE * sin( dAng * (float)i ) ) );
      }
      for ( unsigned int i = 1; i < points.size(); ++i )
      {
         m_rotationAxes->add( LineSegment( points[i - 1], points[i], oxColor ) );
      }

      
      // OY
      points.clear();
      m_rotationAxes->add( LineSegment( D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, SIZE, 0 ), oyColor ) );
      points.push_back( D3DXVECTOR3( SIZE, 0, 0 ) );
      for ( unsigned int i = 0; i < 17; ++i )
      {
         points.push_back( D3DXVECTOR3( SIZE * cos( dAng * (float)i ), 0, SIZE * sin( dAng * (float)i ) ) );
      }
      for ( unsigned int i = 1; i < points.size(); ++i )
      {
         m_rotationAxes->add( LineSegment( points[i - 1], points[i], oyColor ) );
      }


      // OZ
      points.clear();
      m_rotationAxes->add( LineSegment( D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, 0, SIZE ), ozColor ) );
      points.push_back( D3DXVECTOR3( 0, SIZE, 0 ) );
      for ( unsigned int i = 0; i < 17; ++i )
      {
         points.push_back( D3DXVECTOR3( SIZE * sin( dAng * (float)i ), SIZE * cos( dAng * (float)i ), 0 ) );
      }
      for ( unsigned int i = 1; i < points.size(); ++i )
      {
         m_rotationAxes->add( LineSegment( points[i - 1], points[i], ozColor ) );
      }

      m_rotationAxes->rebuild();
   }

   // prepare the geometry of scaling axes
   m_scalingAxes = dynamic_cast< LineSegments*>( rm.findResource( "ScalingAxes" ) );
   if ( !m_scalingAxes )
   {
      m_scalingAxes = new LineSegments( "ScalingAxes" );
      rm.addResource( m_scalingAxes );

      // OX
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( 0, 0, 0 ),      D3DXVECTOR3( SIZE,  0,  0 ), oxColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( SIZE,  1, -1 ), D3DXVECTOR3( SIZE, -1, -1 ), oxColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( SIZE, -1, -1 ), D3DXVECTOR3( SIZE, -1,  1 ), oxColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( SIZE, -1,  1 ), D3DXVECTOR3( SIZE,  1,  1 ), oxColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( SIZE,  1,  1 ), D3DXVECTOR3( SIZE,  1, -1 ), oxColor ) );

      // OY
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( 0, 0, 0 ),      D3DXVECTOR3(  0, SIZE,  0 ), oyColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3(  1, SIZE, -1 ), D3DXVECTOR3( -1, SIZE, -1 ), oyColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( -1, SIZE, -1 ), D3DXVECTOR3( -1, SIZE,  1 ), oyColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( -1, SIZE,  1 ), D3DXVECTOR3(  1, SIZE,  1 ), oyColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3(  1, SIZE,  1 ), D3DXVECTOR3(  1, SIZE, -1 ), oyColor ) );

      // OZ
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( 0, 0, 0 ),      D3DXVECTOR3(  0,  0, SIZE ), ozColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3(  1, -1, SIZE ), D3DXVECTOR3( -1, -1, SIZE ), ozColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( -1, -1, SIZE ), D3DXVECTOR3( -1,  1, SIZE ), ozColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3( -1,  1, SIZE ), D3DXVECTOR3(  1,  1, SIZE ), ozColor ) );
      m_scalingAxes->add( LineSegment( D3DXVECTOR3(  1,  1, SIZE ), D3DXVECTOR3(  1, -1, SIZE ), ozColor ) );

      m_scalingAxes->rebuild();
   }

   // create the renderable and attach it
   m_renderable = new Renderable( "GizmoRenderable" );
   add( m_renderable );

   // create a rendering effect
   m_effect = new GizmoEffect( rm, camera, node );
   m_renderable->add( m_effect );

   // set the initial mode
   setMode( GM_TRANSLATION );
}

///////////////////////////////////////////////////////////////////////////////

Gizmo::~Gizmo()
{
   m_translationAxes = NULL;
   m_rotationAxes = NULL;
   m_scalingAxes = NULL;
   m_effect = NULL;
   m_renderable = NULL;
   m_geometry = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::setMode( Mode mode )
{
   if ( m_geometry )
   {
      // reset the old geometry
      m_renderable->remove( *m_geometry );
      m_geometry = NULL;
   }

   // set the new mode and the related geometry
   m_mode = mode;
   switch( mode )
   {
   case GM_TRANSLATION:
      {
         m_geometry = new Geometry( *m_translationAxes );
         break;
      }

   case GM_ROTATION:
      {
         m_geometry = new Geometry( *m_rotationAxes );
         break;
      }

   case GM_SCALING:
      {
         m_geometry = new Geometry( *m_scalingAxes );
         break;
      }
   }

   m_renderable->add( m_geometry );
}

///////////////////////////////////////////////////////////////////////////////
