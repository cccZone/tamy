#include "core-Renderer\Geometry.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\Camera.h"
#include "core-MVC\SpatialEntity.h"
#include "core-MVC.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT(Geometry, Entity)
   PROPERTY_EDIT( "resource", GeometryResource*, m_resource )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry()
   : m_resource(NULL)
   , m_camera( NULL )
   , m_parentNode( NULL )
   , m_visible( true )
{
}

///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry(GeometryResource& resource)
   : m_resource( &resource )
   , m_camera( NULL )
   , m_parentNode( NULL )
   , m_visible( true )
{
}

///////////////////////////////////////////////////////////////////////////////

Geometry::~Geometry()
{
   m_resource = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::render()
{
   if ( m_resource && m_camera && m_parentNode )
   {
      onPreRender();
      m_resource->render();
      onPostRender();
   }
}

///////////////////////////////////////////////////////////////////////////////

BoundingVolume* Geometry::calculateBoundingVolume() const
{
   BoundingVolume* vol = NULL;
   if ( m_resource )
   {
      if ( m_parentNode )
      {
         vol = m_resource->getBoundingVolume() * m_parentNode->getGlobalMtx();
      }
      else
      {
         D3DXMATRIX identityMtx;
         D3DXMatrixIsIdentity( &identityMtx );
         vol = m_resource->getBoundingVolume() * identityMtx;
      }
   }
   else
   {
      vol = new AABoundingBox();
   }

   return vol;
}

///////////////////////////////////////////////////////////////////////////////

std::string Geometry::getGeometryName() const
{
   if ( m_resource )
   {
      return m_resource->getResourceName();
   }
   else
   {
      return "<<uninitialized geometry>>";
   }
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onAttached( Model& hostModel ) 
{
   m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onDetached( Model& hostModel ) 
{
   m_camera = NULL;
}


///////////////////////////////////////////////////////////////////////////////

void Geometry::onAttached( Entity& parent )
{
   m_parentNode = dynamic_cast< SpatialEntity* >( &parent );
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onDetached( Entity& parent )
{
   m_parentNode = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onComponentAdded( Component< Model >& component )
{
   ModelComponent< Camera >* comp = dynamic_cast< ModelComponent< Camera >* >( &component );
   if ( comp )
   {
      m_camera = &comp->get();
   }
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onObjectLoaded()
{
   __super::onObjectLoaded();

   if ( isAttached() )
   {
      m_parentNode = dynamic_cast< SpatialEntity *>( &getParent() );
   }
}

///////////////////////////////////////////////////////////////////////////////
