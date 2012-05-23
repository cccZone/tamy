#include "QueryRenderingPass.h"
#include "core.h"
#include "core-Renderer.h"
#include "QueryableEntity.h"
#include "SceneQuery.h"

// representations
#include "QueryableGeometry.h"


///////////////////////////////////////////////////////////////////////////////

QueryRenderingPass::QueryRenderingPass()
   : m_sceneSnapshot( NULL )
   , m_shader( NULL )
   , m_debugMode( false )
{
   // define associations
   associateAbstract< Geometry, QueryableGeometry > ();
}

///////////////////////////////////////////////////////////////////////////////

QueryRenderingPass::~QueryRenderingPass()
{
   resetContents();

   m_queriesList.clear();
   m_completedQueriesList.clear();
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::onEntityAdded( Entity& entity )
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

void QueryRenderingPass::onEntityRemoved( Entity& entity )
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

void QueryRenderingPass::onEntityChanged( Entity& entity )
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::resetContents()
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
   static FilePath shaderName( "Editor/Shaders/SceneQueryEffect.psh" );
   ResourcesManager& rm = ResourcesManager::getInstance();
   m_shader = rm.findResource< PixelShader >( shaderName );
   if ( !m_shader )
   {
      m_shader = new PixelShader( shaderName );
      
      // load the shader code
      File* shaderFile = rm.getFilesystem().open( shaderName, std::ios_base::in );
      StreamBuffer< char > shaderCodeBuf( *shaderFile );
      m_shader->setScript( shaderCodeBuf.getBuffer() );
      delete shaderFile;

      rm.addResource( m_shader );
   }

   // create the render target
   delete m_sceneSnapshot;
   m_sceneSnapshot = new RenderTarget( new RTSPDynamic( renderer ), TU_COLOR, true, Color( 0, 0, 0, 0 ) );
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::deinitialize( Renderer& renderer )
{
   m_shader = NULL;
   delete m_sceneSnapshot; m_sceneSnapshot = NULL;
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

   if ( m_debugMode )
   {
      // if the debug mode is enabled, draw the debug version of the scene
      new ( renderer() ) RCActivateRenderTarget( NULL );
      for ( Representations::iterator it = m_representations.begin(); it != m_representations.end(); ++it )
      {
         it->second->render( renderer, *m_shader );
      }
   }

   // check if there are any new queries
   if ( m_queriesList.empty() )
   {
      // don't render anything if there were no queries made
      return;
   }

   // set the rendering target
   new ( renderer() ) RCActivateRenderTarget( m_sceneSnapshot );

   // draw the representations
   for ( Representations::iterator it = m_representations.begin(); it != m_representations.end(); ++it )
   {
      it->second->render( renderer, *m_shader );
   }

   // create the commands that will get the queried results
   for ( QueriesList::iterator it = m_queriesList.begin(); it != m_queriesList.end(); ++it )
   {
      new ( renderer() ) RCGetPixel( *m_sceneSnapshot, (*it)->getQueriedPosition(), (Color&)(*it)->getResultBuffer() );
   }

   // remove all queries from the list - we have fulfilled them
   m_completedQueriesList = m_queriesList;
   m_queriesList.clear();
}

///////////////////////////////////////////////////////////////////////////////

void QueryRenderingPass::ptrToVec( void* ptr, Vector& outVec  )
{
   PtrAsBytes ptrRep;
   ptrRep.ptr = (long)ptr;

   outVec.x = ( float )( ptrRep.b[0] / 255.f );
   outVec.y = ( float )( ptrRep.b[1] / 255.f );
   outVec.z = ( float )( ptrRep.b[2] / 255.f );
   outVec.w = ( float )( ptrRep.b[3] / 255.f );
}

///////////////////////////////////////////////////////////////////////////////

void* QueryRenderingPass::vecToPtr( const Vector& vec )
{
   PtrAsBytes ptrRep;
   unsigned int r = ( unsigned int )( vec.x * 255.f );
   unsigned int g = ( unsigned int )( vec.y * 255.f );
   unsigned int b = ( unsigned int )( vec.z * 255.f );
   unsigned int a = ( unsigned int )( vec.w * 255.f );

   ptrRep.b[0] = r;
   ptrRep.b[1] = g;
   ptrRep.b[2] = b;
   ptrRep.b[3] = a;

   return ( void* )( ptrRep.ptr );
}

///////////////////////////////////////////////////////////////////////////////
