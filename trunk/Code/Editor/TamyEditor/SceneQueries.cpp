#include "SceneQueries.h"
#include "TamyEditor.h"
#include "core-MVC.h"
#include "core-Renderer.h"
#include "QueryRenderingPass.h"


///////////////////////////////////////////////////////////////////////////////

SceneQueries::SceneQueries()
: m_currObservedScene( NULL )
, m_sceneSnapshot( NULL )
, m_renderingPass( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

SceneQueries::~SceneQueries()
{
   resetContents();

   delete m_sceneSnapshot; m_sceneSnapshot = NULL;
   m_renderingPass = NULL;

   m_currObservedScene = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::initialize( TamyEditor& mgr )
{
   ASSERT( m_sceneSnapshot == NULL, "SceneQueries is already initialized" );

   // register new service
   mgr.registerService< SceneQueries >( *this, *this );

   // create the texture to which the snapshot of a scene will be made
   Renderer& renderer = mgr.requestService< Renderer >();
   m_sceneSnapshot = new RenderTarget( &renderer, true, Color( 0, 0, 0, 0 ) );

   // register a rendering pass
   CompositeRenderingMechanism& compRenderingMech = mgr.requestService< CompositeRenderingMechanism >();
   ResourcesManager& rm = mgr.requestService< ResourcesManager >();
   m_renderingPass = new QueryRenderingPass( *m_sceneSnapshot, rm );
   compRenderingMech.add( "SceneQueries", m_renderingPass );
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::onServiceRegistered( TamyEditor& mgr )
{
   if ( m_currObservedScene )
   {
      m_currObservedScene->detach( *this );
   }

   if ( mgr.needsUpdate< Model >( *m_currObservedScene ) )
   {
      if ( mgr.hasService< Model >() )
      {
         m_currObservedScene = &mgr.requestService< Model >();
      }
      else
      {
         m_currObservedScene = NULL;
      }
   }

   if ( m_currObservedScene )
   {
      m_currObservedScene->attach( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::onEntityAdded( Entity& entity )
{
   if ( m_renderingPass )
   {
      m_renderingPass->addEntity( entity );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::onEntityRemoved( Entity& entity )
{
   if ( m_renderingPass )
   {
      m_renderingPass->removeEntity( entity );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::onEntityChanged( Entity& entity )
{
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::resetContents()
{
   if ( m_renderingPass )
   {
      m_renderingPass->reset();
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::query( SceneQuery& query )
{
   if ( m_renderingPass )
   {
      m_renderingPass->query( query );
   }
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR4 SceneQueries::ptrToVec( void* ptr )
{
   PtrAsBytes ptrRep;
   ptrRep.ptr = (long)ptr;

   D3DXVECTOR4 vec(
      ( float )( ptrRep.b[0] / 255.f ),
      ( float )( ptrRep.b[1] / 255.f ),
      ( float )( ptrRep.b[2] / 255.f ),
      ( float )( ptrRep.b[3] / 255.f )
      );
   return vec;
}

///////////////////////////////////////////////////////////////////////////////

void* SceneQueries::vecToPtr( const D3DXVECTOR4& vec )
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
