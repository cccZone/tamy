#include "core-Renderer\Geometry.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\RenderState.h"
#include "core-MVC\SpatialEntity.h"
#include "core-MVC.h"
#include "core.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( Geometry, SpatialEntity )
   PROPERTY_EDIT( "resource", GeometryResource*, m_resource )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry()
   : m_resource(NULL)
   , m_parentNode( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry( GeometryResource& resource )
   : m_resource( &resource )
   , m_parentNode( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

Geometry::~Geometry()
{
   m_resource = NULL;
   m_states.clear();
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::render( Renderer& renderer )
{
   if ( m_resource )
   {
      onPreRender( renderer );
      m_resource->render( renderer );
      onPostRender( renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::addState( RenderState& state )
{
   // we don't want duplicates - so look for one
   RenderStatesVec::const_iterator it = std::find( m_states.begin(), m_states.end(), &state );
   if ( it == m_states.end() )
   {
      // add the state
      m_states.push_back( &state );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::removeState( RenderState& state )
{
   // find the position of the requested state
   RenderStatesVec::const_iterator it = std::find( m_states.begin(), m_states.end(), &state );
   if ( it != m_states.end() )
   {
      // remove the state
      m_states.erase( it );
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

void Geometry::onObjectLoaded()
{
   __super::onObjectLoaded();

   if ( isAttached() )
   {
      m_parentNode = dynamic_cast< SpatialEntity *>( &getParentNode() );
   }
}

///////////////////////////////////////////////////////////////////////////////
