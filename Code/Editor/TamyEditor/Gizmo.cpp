#include "Gizmo.h"
#include "core-Renderer.h"
#include "core.h"
#include "core-MVC.h"
#include "GizmoAxis.h"
#include "GizmoMaterial.h"
#include "DebugGeometryBuilder.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT(Gizmo)
   PARENT( SpatialEntity )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Gizmo::Gizmo( SpatialEntity& node, Mode mode, Camera& activeCamera )
   : m_node( node )
   , m_mode( (Mode)-1 )
   , OX_COLOR( 1, 0, 0, 1 )
   , OY_COLOR( 0, 1, 0, 1 )
   , OZ_COLOR( 0, 0, 1, 1 )
   , m_translationOp( new TranslationGizmoOp() )
   , m_rotationOp( new RotationGizmoOp() )
   , m_scalingOp( new ScalingGizmoOp() )
{
   // create materials for particular axes
   {
      FilePath gizmoShaderPath( "Editor/Shaders/Gizmo.tpsh" );
      PixelShader* shader = ResourcesManager::getInstance().create< PixelShader >( gizmoShaderPath );
      ASSERT_MSG( shader != NULL, "Gizmo shader doesn't exist" );

      m_axisMaterial[0] = new GizmoMaterial( 0, OX_COLOR, *shader );
      m_axisMaterial[1] = new GizmoMaterial( 1, OY_COLOR, *shader );
      m_axisMaterial[2] = new GizmoMaterial( 2, OZ_COLOR, *shader );
   }

   // create gizmo axes
   for ( byte i = 0; i < 3; ++i )
   {
      m_meshes[i] = NULL;
      m_geometry[i] = new GizmoAxis( i, m_node, activeCamera );
      m_geometry[i]->addState( *( m_axisMaterial[i] ) );
      add( m_geometry[i] );
   }

   // set the initial mode
   setMode( mode );
}

///////////////////////////////////////////////////////////////////////////////

Gizmo::~Gizmo()
{
   // remove the materials, since they are not managed
   for ( byte i = 0; i < 3; ++i )
   {
      SAFE_DELETE( m_axisMaterial[i] );
   }

   // geometries will be removed by this entity
   for ( byte i = 0; i < 3; ++i )
   {
      SAFE_DELETE( m_meshes[i] );

      m_geometry[i] = NULL;
   }

   delete m_translationOp;
   m_translationOp = NULL;

   delete m_rotationOp;
   m_rotationOp = NULL;

   delete m_scalingOp;
   m_scalingOp = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::setMode( Mode mode )
{
   if ( m_mode == mode )
   {
      return;
   }

   // set the new mode and the related geometry
   m_mode = mode;

   switch( m_mode )
   {
   case GM_TRANSLATION:
      {
         createTranslationGizmo();
         break;
      }

   case GM_ROTATION:
      {
         createRotationGizmo();
         break;
      }

   case GM_SCALING:
      {
         createScalingGizmo();
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::createTranslationGizmo()
{
   const Matrix& mtx = m_node.getGlobalMtx();

   float armLength = 0.2f;
   float armHalfDim = 0.003f;
   Vector ox, oy, oz;
   ox.setMul( Vector::OX, armLength );
   oy.setMul( Vector::OY, armLength );
   oz.setMul( Vector::OZ, armLength );

   // OX
   delete m_meshes[0];
   m_meshes[0] = DebugGeometryBuilder::createArrow( armHalfDim, Vector::ZERO, ox );
   m_geometry[0]->setMesh( *m_meshes[0] );
   m_geometry[0]->setOperation( *m_translationOp );

   // OY
   delete m_meshes[1];
   m_meshes[1] = DebugGeometryBuilder::createArrow( armHalfDim, Vector::ZERO, oy );
   m_geometry[1]->setMesh( *m_meshes[1] );
   m_geometry[1]->setOperation( *m_translationOp );

   // OZ
   delete m_meshes[2];
   m_meshes[2] = DebugGeometryBuilder::createArrow( armHalfDim, Vector::ZERO, oz );
   m_geometry[2]->setMesh( *m_meshes[2] );
   m_geometry[2]->setOperation( *m_translationOp );
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::createRotationGizmo()
{
   const Matrix& mtx = m_node.getGlobalMtx();
   Quaternion rotQ;

   float ringRadius = 0.2f;
   float ringCircumferenceWidth = 0.003f;

   Matrix transformOX, transformOY, transformOZ;

   // calculate transform OX
   {
      rotQ.setAxisAngle( Vector::OY, DEG2RAD( 90.0f ) );
      transformOX.setRotation( rotQ );
   }

   // calculate transform OY
   {
      rotQ.setAxisAngle( Vector::OX, DEG2RAD( 90.0f ) );
      transformOY.setRotation( rotQ );
   }

   // calculate transform OZ
   {
      rotQ.setAxisAngle( Vector::OZ, DEG2RAD( 90.0f ) );
      transformOZ.setRotation( rotQ );
   }

   const uint segmentsCount = 56;
   const uint segmentVerticesCount = 3;
   float innerRadius = ringRadius - ringCircumferenceWidth * 0.5f;
   float outerRadius = ringRadius + ringCircumferenceWidth * 0.5f;

   // OX
   delete m_meshes[0];
   m_meshes[0] = DebugGeometryBuilder::createTorus( innerRadius, outerRadius, transformOX, segmentsCount, segmentVerticesCount );
   m_geometry[0]->setMesh( *( m_meshes[0] ) );
   m_geometry[0]->setOperation( *m_rotationOp );
   
   // OY
   delete m_meshes[1];
   m_meshes[1] = DebugGeometryBuilder::createTorus( innerRadius, outerRadius, transformOY, segmentsCount, segmentVerticesCount );
   m_geometry[1]->setMesh( *( m_meshes[1] ) );
   m_geometry[1]->setOperation( *m_rotationOp );

   // OZ
   delete m_meshes[2];
   m_meshes[2] = DebugGeometryBuilder::createTorus( innerRadius, outerRadius, transformOZ, segmentsCount, segmentVerticesCount );
   m_geometry[2]->setMesh( *( m_meshes[2] ) );
   m_geometry[2]->setOperation( *m_rotationOp );
}

///////////////////////////////////////////////////////////////////////////////

void Gizmo::createScalingGizmo()
{
   const Matrix& mtx = m_node.getGlobalMtx();

   float armLength = 0.2f;
   float armHalfDim = 0.003f;
   Vector ox, oy, oz;
   ox.setMul( Vector::OX, armLength );
   oy.setMul( Vector::OY, armLength );
   oz.setMul( Vector::OZ, armLength );

   // OX
   delete m_meshes[0];
   m_meshes[0] = DebugGeometryBuilder::createBoxHeadedArrow( armHalfDim, Vector::ZERO, ox );
   m_geometry[0]->setMesh( *m_meshes[0] );
   m_geometry[0]->setOperation( *m_scalingOp );

   // OY
   delete m_meshes[1];
   m_meshes[1] = DebugGeometryBuilder::createBoxHeadedArrow( armHalfDim, Vector::ZERO, oy );
   m_geometry[1]->setMesh( *m_meshes[1] );
   m_geometry[1]->setOperation( *m_scalingOp );

   // OZ
   delete m_meshes[2];
   m_meshes[2] = DebugGeometryBuilder::createBoxHeadedArrow( armHalfDim, Vector::ZERO, oz );
   m_geometry[2]->setMesh( *m_meshes[2] );
   m_geometry[2]->setOperation( *m_scalingOp );
}

///////////////////////////////////////////////////////////////////////////////
