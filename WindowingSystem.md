# Windowing system #

The windowing system lets you create and manage application windows in a platform-independent manner.


## Window builder ##
Use the `WindowBuilder` instance to create the application window.

```
MAIN_FUNCTION_START()
{
   const uint windowWidth = 800;
   const uint windowHeight = 600;
   const bool fullScreen = false;
   const char* appName = "WinGame";

   // create the main application window
   WindowHandle mainWindow = WindowBuilder::createWindow( appName, fullScreen, windowWidth, windowHeight ); 

   // your application code goes here

}
MAIN_FUNCTION_END()
```

You need to use the specified macros `MAIN_FUNCTION_START` and `MAIN_FUNCTION_END` instead of defining your own function `main` in order to be able to use the `WindowBuilder` and the whole tamy `Application` system for that matter.