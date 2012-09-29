#include "core-Renderer\Geometry.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\RenderState.h"
#include "core-Renderer\VertexShaderConfigurator.h"
#include "core-MVC\SpatialEntity.h"
#include "core-MVC.h"
#include "core.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( Geometry );
   PARENT( SpatialEntity );
   PROPERTY_EDIT( "resource", GeometryResource*, m_resource );
END_OBJECT();


///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry( const std::string& name )
   : SpatialEntity( name )
   , m_resource(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry( const Geometry& rhs )
   : SpatialEntity( rhs )
   , m_resource( rhs.m_resource )
{
   setBoundingVolume( m_resource->getBoundingVolume().clone() );
}

///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry( GeometryResource& resource, const std::string& name )
   : SpatialEntity( name )
   , m_resource( &resource )
{
   setBoundingVolume( m_resource->getBoundingVolume().clone() );

}

///////////////////////////////////////////////////////////////////////////////

Geometry::~Geometry()
{
   m_resource = NULL;
   m_states.clear();
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::render( Renderer& renderer, VertexShaderConfigurator* externalConfigurator )
{
   if ( m_resource )
   {
      RCBindVertexShader* vsComm = onPreRender( renderer );
      if ( vsComm )
      {
         if ( externalConfigurator )
         {
            externalConfigurator->configure( *this, vsComm );
         }

         m_resource->render( renderer );
         onPostRender( renderer );
      }
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
      std::sort( m_states.begin(), m_states.end(), StateComparator() );
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

void Geometry::getBoundingBox( AABoundingBox& boundingBox ) const
{
   const BoundingVolume& boundingVol = getBoundingVolume();
   boundingVol.calculateBoundingBox( boundingBox );
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::setMesh( GeometryResource& mesh )
{
   m_resource = &mesh;

   setBoundingVolume( m_resource->getBoundingVolume().clone() );

}

///////////////////////////////////////////////////////////////////////////////

std::string Geometry::getMeshName() const
{
   if ( m_resource )
   {
      return m_resource->getFilePath();
   }
   else
   {
      return "<<uninitialized geometry>>";
   }
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onChildAttached( Entity& child )
{
   __super::onChildAttached( child );

   RenderState* renderState = DynamicCast< RenderState >( &child );
   if ( renderState )
   {
      addState( *renderState );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onChildDetached( Entity& child )
{
   __super::onChildDetached( child );

   RenderState* renderState = DynamicCast< RenderState >( &child );
   if ( renderState )
   {
      removeState( *renderState );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onObjectLoaded()
{
   __super::onObjectLoaded();

   // refresh the global bounds
   setBoundingVolume( m_resource->getBoundingVolume().clone() );
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_resource" )
   {
      setBoundingVolume( m_resource->getBoundingVolume().clone() );
   }
}

///////////////////////////////////////////////////////////////////////////////

Entity* Geometry::cloneSelf() const
{
   Geometry* entity = new Geometry( *this );
   return entity;
}

///////////////////////////////////////////////////////////////////////////////
