#include "DRGeometry.h"
#include "core-Renderer/Geometry.h"
#include "core-Renderer/TriangleMesh.h"
#include "DebugGeometryBuilder.h"
#include "SingleColorDebugMat.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( DRGeometry )
PARENT( DebugGeometry )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

DRGeometry::DRGeometry( Geometry& geometry )
   : DebugGeometry( &geometry )
   , m_geometry( geometry )
   , m_mesh( NULL )
{
   AABoundingBox bb;
   const BoundingVolume& bounds = geometry.getMesh()->getBoundingVolume();
   bounds.calculateBoundingBox( bb );

   m_mesh = DebugGeometryBuilder::createBox( bb.min, bb.max );
   m_material = new SingleColorDebugMat( Color( 255, 242, 61 ), true );

   setMesh( *m_mesh );
}

///////////////////////////////////////////////////////////////////////////////

DRGeometry::~DRGeometry()
{
   m_mesh->removeReference();
   m_mesh = NULL;

   m_material->removeReference();
   m_material = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DRGeometry::enableBoundingBox( bool enable )
{
   if ( enable )
   {
      addState( *m_material );
   }
   else
   {
      removeState( *m_material );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DRGeometry::enableDebugShape( bool enable )
{
}

///////////////////////////////////////////////////////////////////////////////
