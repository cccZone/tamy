#include "DRDirectionalLight.h"
#include "core-Renderer/DirectionalLight.h"
#include "core-Renderer/Geometry.h"
#include "core-Renderer/TriangleMesh.h"
#include "DebugGeometryBuilder.h"
#include "SingleColorDebugMat.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( DRDirectionalLight )
   PARENT( DebugGeometry )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

DRDirectionalLight::DRDirectionalLight( DirectionalLight& light )
   : DebugGeometry( &light )
   , m_light( light )
   , m_mesh( NULL )
{
   m_mesh = DebugGeometryBuilder::createTorus( 0.5f, 1.0f, Matrix::IDENTITY, 6, 5 );
   m_material = new SingleColorDebugMat( Color( 255, 242, 61 ) );

   setMesh( *m_mesh );
   addState( *m_material );
}

///////////////////////////////////////////////////////////////////////////////

DRDirectionalLight::~DRDirectionalLight()
{
   m_mesh->removeReference();
   m_mesh = NULL;

   m_material->removeReference();
   m_material = NULL;
}

///////////////////////////////////////////////////////////////////////////////
