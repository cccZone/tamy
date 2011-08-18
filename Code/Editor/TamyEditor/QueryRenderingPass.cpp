#include "QueryRenderingPass.h"
#include "core.h"
#include "core-Renderer.h"
#include "QueryableEntity.h"
#include "SceneQuery.h"
#include "SceneQueries.h"

// representations
#include "QueryableGeometry.h"


///////////////////////////////////////////////////////////////////////////////

QueryRenderingPass::QueryRenderingPass( RenderTarget& sceneSnapshot )
   : m_sceneSnapshot( sceneSnapshot )
   , m_shader( NULL )
{
   // define associations
   associateAbstract< Geometry, QueryableGeometry > ();
}

///////////////////////////////////////////////////////////////////////////////

QueryRenderingPass::~QueryRenderingPass()
{
   reset();

   m_queriesList.clear();
   m_completedQueriesList.clear();
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
   if ( representation != NULL )
   {
      m_representations.insert( std::make_pair( &entity, representation ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::removeEntity( Entity& entity )
{
   Representations::iterator it = m_representations.find( &entity );
   if ( it != m_representations.end() )
   {
      QueryableEntity* repr = it->second;
      m_representations.erase( it );

      delete repr;
   }
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::reset()
{
   for ( Representations::iterator it = m_representations.begin(); it != m_representations.end(); ++it )
   {
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

void QueryRenderingPass::initialize( Renderer& renderer )
{
   // load the shader
   static const char* shaderName = "Editor/Shaders/SceneQueryEffect.psh";
   ResourcesManager& rm = ResourcesManager::getInstance();
   m_shader = dynamic_cast< PixelShader* >( rm.findResource( "SceneQueryEffect" ) );
   if ( !m_shader )
   {
      m_shader = new PixelShader( "SceneQueryEffect" );
      m_shader->loadFromFile( rm.getFilesystem(), shaderName );
      rm.addResource( m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::deinitialize( Renderer& renderer )
{
   delete m_shader; m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::render( Renderer& renderer )
{
   // notify about the completed queries
   for ( QueriesList::iterator it = m_completedQueriesList.begin(); it != m_completedQueriesList.end(); ++it )
   {
      (*it)->notifyResult();
   }
   m_completedQueriesList.clear();

   // check if there are any new queries
   if ( m_queriesList.empty() )
   {
      // don't render anything if there were no queries made
      return;
   }

   // draw the representations
   for ( Representations::iterator it = m_representations.begin(); it != m_representations.end(); ++it )
   {
      it->second->render( renderer, *m_shader );
   }

   // create the commands that will get the queried results
   for ( QueriesList::iterator it = m_queriesList.begin(); it != m_queriesList.end(); ++it )
   {
      new ( renderer() ) RCGetPixel( m_sceneSnapshot, (*it)->getQueriedPosition(), (*it)->getResultBuffer() );
   }

   // remove all queries from the list - we have fulfilled them
   m_completedQueriesList = m_queriesList;
   m_queriesList.clear();
}

///////////////////////////////////////////////////////////////////////////////
