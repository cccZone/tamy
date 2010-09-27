#include "QueryRenderingPass.h"
#include "core.h"
#include "core-Renderer.h"
#include "QueryableEntity.h"
#include "SceneQuery.h"
#include "SceneQueryEffect.h"
#include "SceneQueries.h"

// representations
#include "QueryableGeometry.h"


///////////////////////////////////////////////////////////////////////////////

QueryRenderingPass::QueryRenderingPass( RenderTarget& sceneSnapshot, ResourcesManager& rm )
: RenderingPass( &sceneSnapshot )
, m_sceneSnapshot( sceneSnapshot )
{
   // create the query effect
   m_effect = new SceneQueryEffect( rm );

   // define associations
   associateAbstract< Geometry, QueryableGeometry > ();
}

///////////////////////////////////////////////////////////////////////////////

QueryRenderingPass::~QueryRenderingPass()
{
   reset();

   delete m_effect; m_effect = NULL;
   m_queriesList.clear();
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::addEntity( Entity& entity )
{
   Representations::iterator reprIt = m_representations.find( &entity );
   if ( reprIt != m_representations.end() )
   {
      // same entity instance can't have more than one representation
      return;
   }

   QueryableEntity* representation = create( entity );
   if (representation != NULL)
   {
      representation->initialize( *m_effect );
      m_representations.insert( std::make_pair( &entity, representation ) );
      statesMgr().add( *representation );
   }
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::removeEntity( Entity& entity )
{
   Representations::iterator it = m_representations.find(&entity);
   if (it != m_representations.end())
   {
      QueryableEntity* repr = it->second;
      statesMgr().remove( *repr );
      m_representations.erase(it);

      delete repr;
   }
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::reset()
{
   for ( Representations::iterator it = m_representations.begin();
      it != m_representations.end(); ++it )
   {
      statesMgr().remove( *it->second );
      delete it->second;
   }
   m_representations.clear();
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::query( SceneQuery& query )
{
   m_queriesList.push_back( &query );
}

///////////////////////////////////////////////////////////////////////////////

bool QueryRenderingPass::onPreRender()
{
   return !m_queriesList.empty();
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::onPostRender()
{
   for ( QueriesList::iterator it = m_queriesList.begin(); it != m_queriesList.end(); ++it )
   {
      const D3DXVECTOR2& queryPos = (*it)->getQueriedPosition();
      Entity* entityAtPos = getEntityFromPos( queryPos );
      (*it)->setResult( entityAtPos );
   }

   // remove all queries from the list - we have fulfilled them
   m_queriesList.clear();
}

///////////////////////////////////////////////////////////////////////////////

Entity* QueryRenderingPass::getEntityFromPos( const D3DXVECTOR2& pos )
{
   D3DXVECTOR4 value = ( D3DXVECTOR4 )( m_sceneSnapshot.getPixel( pos ) );
   Entity* entity = reinterpret_cast< Entity* >( SceneQueries::vecToPtr( value ) );

   return entity;
}

///////////////////////////////////////////////////////////////////////////////
