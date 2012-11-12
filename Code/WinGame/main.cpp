#include "core.h"
#include "core-AppFlow.h"
#include "win-AppFlow.h"
#include "GameApplication.h"


///////////////////////////////////////////////////////////////////////////////

// type includes
#include "core/TypesRegistry.cpp"
#include "core-MVC/TypesRegistry.cpp"
#include "core-Renderer/TypesRegistry.cpp"
#include "core-AI/TypesRegistry.cpp"
#include "core-AppFlow/TypesRegistry.cpp"



///////////////////////////////////////////////////////////////////////////////

MAIN_FUNCTION_START()
{
   const uint windowWidth = 800;
   const uint windowHeight = 600;
   const bool fullScreen = false;
   const char* appName = "WinGame";

   // create the main application window
   WindowHandle mainWindow = WindowBuilder::createWindow( appName, fullScreen, windowWidth, windowHeight );


   // create the application and plug it into the manager
   GameApplication* application = new GameApplication( windowWidth, windowHeight, fullScreen, mainWindow );
   ApplicationManager& appMgr = WindowBuilder::getApplicationManager();
   appMgr.addApplication( *application );
   appMgr.setEntryApplication( application->getName() );

   // run the main loop
   while( appMgr.step() )
   {
   }

   // cleanup
   delete application;

   MaterialsDB::deinitialize();
   ResourcesManager::deinitialize();
   IDStringsPool::deinitialize();
   Profiler::deinitialize();
   ReflectionTypesRegistry::deinitialize();
   MemoryRouter::deinitialize();

   return 0;
}
MAIN_FUNCTION_END()
   
///////////////////////////////////////////////////////////////////////////////
