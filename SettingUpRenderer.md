# Setting up a renderer #


  * Select the renderer implementation ( DX9, OpenGL, etc. ) and link against it ( in these examples I'm going to use DX9 implementation

  * Initialize the rendering library

```
if ( s_d3d9 == NULL )
{
   s_d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
   if ( s_d3d9 == NULL )
   {
      ASSERT_MSG( false, "Cannot initialize DirectX 9 library" );
      return -1;
   }
}
```

  * Create a rendering device description

```
SimpleTamyConfigurator configurator;
RenderingDevice* device = configurator.selectRenderingDevice( *s_d3d9, 800, 600, false );
```

  * Finally - create a renderer that runs on the specified device ( I'm assuming you already have a handle to the window you want the renderer to render to - m\_hWnd )

```

DX9Initializer initializer( *s_d3d9 );
m_renderer = initializer.createDisplay( DX9Settings(*device), m_hWnd );

// get rid of the device descrtiption - we no longer require it
delete device;
```


  * Having created the renderer, you can now set up the rendering mechanism. In this case we're gonna load a rendering pipeline and plug it in as a rendering mechanism:

```
ResourcesManager& resMgr = ResourcesManager::getInstance();
 
// load the rendering pipeline
RenderingPipeline* renderingMechanism = resMgr.create< RenderingPipeline >( "renderer.rpl" );
if ( !renderingMechanism )
{
   ASSERT_MSG( renderingMechanism, "Rendering pipeline 'renderer.rpl' doesn't exist" );
   delete renderer;
   return -1;
}
renderer->setMechanism( new RenderingPipelineMechanism( renderingMechanism ) );
```


And that's all. Just be sure to delete the renderer instance once you're done with it :)