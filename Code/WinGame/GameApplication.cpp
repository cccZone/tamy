#include "GameApplication.h"
#include "core-Renderer.h"
#include "dx9-Renderer.h"
#include "core-AppFlow.h"
#include "win-AppFlow\WinWindowBuilder.h"


///////////////////////////////////////////////////////////////////////////////

IDirect3D9* GameApplication::s_d3d9 = NULL;

///////////////////////////////////////////////////////////////////////////////

GameApplication::GameApplication( uint windowWidth, uint windowHeight, bool fullScreen, WindowHandle mainWindow )
   : Application( "game" )
   , m_windowWidth( windowWidth )
   , m_windowHeight( windowHeight )
   , m_fullScreen( fullScreen )
   , m_mainWindow( mainWindow )
   , m_renderer( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

GameApplication::~GameApplication()
{
   delete m_renderer;
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GameApplication::initialize()
{
   // create the renderer
   {
      m_renderer = initializeRenderer( m_fullScreen, m_windowWidth, m_windowHeight, m_mainWindow );
      if ( !m_renderer )
      {
         ASSERT_MSG( m_renderer, "Renderer was not created" );
         return;
      }

      // and register it with the time controller
      TimeControllerTrack& rendererTrack = timeController().add( "renderer" );
      rendererTrack.add( *m_renderer );
   }


   // get the resources manager instance we're gonna be using from now on
   // and set the filesystem root
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   resMgr.getFilesystem().changeRootDir( "./Data/" );

   // load the config resource
   GameConfig* gameConfig = resMgr.create< GameConfig >( FilePath( "game.gcf" ), true );
   if ( !gameConfig )
   {
      ASSERT_MSG( gameConfig, "Game config file was not created" );

      return;
   }


   // load game resources
   {
      // load the rendering pipeline
      RenderingPipelineMechanism* renderingMechanism = NULL;
      {
         RenderingPipeline* renderingPipeline = resMgr.create< RenderingPipeline >( gameConfig->m_renderingPipelinePath, true );
         if ( !renderingPipeline )
         {
            ASSERT_MSG( renderingPipeline, "Rendering pipeline doesn't exist" );
            return;
         }
         renderingMechanism = new RenderingPipelineMechanism( renderingPipeline );
         m_renderer->setMechanism( renderingMechanism );
      }

      // load the game scene
      {
         Model* worldScene = resMgr.create< Model >( gameConfig->m_worldModelPath, true );
         renderingMechanism->addScene( RPS_Main, *worldScene );

         TimeControllerTrack& worldTrack = timeController().add( "world" );
         worldTrack.add( *worldScene );
      }
   }  
}

///////////////////////////////////////////////////////////////////////////////

Renderer* GameApplication::initializeRenderer( bool fullScreen, uint windowWidth, uint windowHeight, WindowHandle mainWindow )
{
   // initialize the renderer library
   if ( s_d3d9 == NULL )
   {
      s_d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
      if ( s_d3d9 == NULL )
      {
         ASSERT_MSG( false, "Cannot initialize DirectX 9 library" );
         return NULL;
      }
   }

   // create a renderer
   SimpleTamyConfigurator configurator;
   RenderingDevice* device = configurator.selectRenderingDevice( *s_d3d9, windowWidth, windowHeight, fullScreen );
   if ( !device )
   {
      return NULL;
   }

   DX9Initializer initializer( *s_d3d9 );
   Renderer* renderer = initializer.createDisplay( DX9Settings(*device), (HWND)mainWindow );
   delete device;

   return renderer;
}

///////////////////////////////////////////////////////////////////////////////

void GameApplication::deinitialize()
{
   delete m_renderer;
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GameApplication::hibernate()
{
}

///////////////////////////////////////////////////////////////////////////////

void GameApplication::dehibernate()
{
}

///////////////////////////////////////////////////////////////////////////////

void GameApplication::notify( const std::string& senderApp, int signalCode )
{
}

///////////////////////////////////////////////////////////////////////////////
