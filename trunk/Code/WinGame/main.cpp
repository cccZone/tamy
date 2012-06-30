#include "core.h"
#include "core-Renderer.h"
#include "dx9-Renderer.h"
#include "core-AppFlow.h"
#include "win-AppFlow\WinWindowBuilder.h"


///////////////////////////////////////////////////////////////////////////////

// type includes
#include "core/TypesRegistry.cpp"
#include "core-MVC/TypesRegistry.cpp"
#include "core-Renderer/TypesRegistry.cpp"
#include "core-AI/TypesRegistry.cpp"
#include "core-AppFlow/TypesRegistry.cpp"

///////////////////////////////////////////////////////////////////////////////

Renderer* initializeRenderer( bool fullScreen, uint windowWidth, uint windowHeight, WindowHandle mainWindow );

// when this flag gets set to 'false', the game will exit
bool g_playGame = true;

///////////////////////////////////////////////////////////////////////////////

MAIN_FUNCTION_START()
{
   const uint windowWidth = 800;
   const uint windowHeight = 600;
   const bool fullScreen = false;
   const char* appName = "WinGame";

   // create the main application window
   WindowHandle mainWindow = WindowBuilder::createWindow( appName, fullScreen, windowWidth, windowHeight ); 

   // initialize subsystems:
   
   // create a time controller instance that's gonna carry out the updates
   // of the game state
   TimeController* timeController = new TimeController();

   // create the renderer
   Renderer* renderer = NULL;
   {
      renderer = initializeRenderer( fullScreen, windowWidth, windowHeight, mainWindow );
      if ( !renderer )
      {
         ASSERT_MSG( renderer, "Renderer was not created" );
         return -1;
      }

      // and register it with the time controller
      TimeControllerTrack& rendererTrack = timeController->add( "renderer" );
      rendererTrack.add( *renderer );
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

      return -1;
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
            delete renderer;
            return -1;
         }
         renderingMechanism = new RenderingPipelineMechanism( renderingPipeline );
         renderer->setMechanism( renderingMechanism );
      }

      // load the game scene
      {
         Model* worldScene = resMgr.create< Model >( gameConfig->m_worldModelPath, true );
         renderingMechanism->addScene( RPS_Main, *worldScene );

         TimeControllerTrack& worldTrack = timeController->add( "world" );
         worldTrack.add( *worldScene );
      }
   }


   // setup timer
   CTimer* mainTime = new CTimer();

   // main loop
   while( g_playGame )
   {
      float timeDelta = mainTime->getTimeElapsed();
      timeController->update( timeDelta );
   }

   // cleanup
   delete mainTime;
   delete timeController;
   delete renderer;

   return 0;
}
MAIN_FUNCTION_END()

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

IDirect3D9* g_d3d9 = NULL;

///////////////////////////////////////////////////////////////////////////////

Renderer* initializeRenderer( bool fullScreen, uint windowWidth, uint windowHeight, WindowHandle mainWindow )
{
   // initialize the renderer library
   if ( g_d3d9 == NULL )
   {
      g_d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
      if ( g_d3d9 == NULL )
      {
         ASSERT_MSG( false, "Cannot initialize DirectX 9 library" );
         return NULL;
      }
   }

   // create a renderer
   SimpleTamyConfigurator configurator;
   RenderingDevice* device = configurator.selectRenderingDevice( *g_d3d9, windowWidth, windowHeight, fullScreen );
   if ( !device )
   {
      return NULL;
   }

   DX9Initializer initializer( *g_d3d9 );
   Renderer* renderer = initializer.createDisplay( DX9Settings(*device), (HWND)mainWindow );
   delete device;

   return renderer;
}

///////////////////////////////////////////////////////////////////////////////