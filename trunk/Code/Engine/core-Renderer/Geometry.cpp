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
   , m_globalBounds( new PointVolume( Vector( 0, 0, 0 ) ) )
{
}

///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry( const Geometry& rhs )
   : SpatialEntity( rhs )
   , m_globalBounds( new PointVolume( Vector( 0, 0, 0 ) ) )
{
}

///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry( GeometryResource& resource, const std::string& name )
   : SpatialEntity( name )
   , m_resource( &resource )
   , m_globalBounds( resource.getBoundingVolume().clone() )
{
}

///////////////////////////////////////////////////////////////////////////////

Geometry::~Geometry()
{
   m_resource = NULL;
   m_states.clear();

   delete m_globalBounds;
   m_globalBounds = NULL;
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
   }

   std::sort( m_states.begin(), m_states.end(), StateComparator() );
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

const BoundingVolume& Geometry::getBoundingVolume() const
{
   if ( m_resource )
   {
      const BoundingVolume& geomBoundingVol = m_resource->getBoundingVolume();

      const Matrix& parentMtx = getGlobalMtx();
      geomBoundingVol.transform( parentMtx, *m_globalBounds );
   }

   return *m_globalBounds;
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::setMesh( GeometryResource& mesh )
{
   m_resource = &mesh;

   delete m_globalBounds;
   m_globalBounds = m_resource->getBoundingVolume().clone();
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

   RenderState* renderState = dynamic_cast< RenderState* >( &child );
   if ( renderState )
   {
      addState( *renderState );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onChildDetached( Entity& child )
{
   __super::onChildDetached( child );

   RenderState* renderState = dynamic_cast< RenderState* >( &child );
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
   delete m_globalBounds;
   if ( m_resource != NULL )
   {
      m_globalBounds = m_resource->getBoundingVolume().clone();
   }
   else
   {
      m_globalBounds = new PointVolume( Vector( 0, 0, 0 ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_resource" )
   {
      delete m_globalBounds;

      if ( m_resource )
      {
         m_globalBounds = m_resource->getBoundingVolume().clone();
      }
      else
      {
         m_globalBounds = new PointVolume( Vector( FLT_MAX, FLT_MAX, FLT_MAX ) );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

Entity* Geometry::cloneSelf() const
{
   Geometry* entity = new Geometry( *this );
   return entity;
}

///////////////////////////////////////////////////////////////////////////////
