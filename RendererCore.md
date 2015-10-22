# Renderer #

A renderer allows to render 3D graphics on screen. It takes care of managing the device's state, finding out about best texture formats etc.

To get it to work, you only need to instantiate a renderer and set a `RenderingMechanism`, that will start feeding the renderer some `RenderCommands` and that's it.

A renderer can't work on its own however - it requres an underlying library it will execute the commands on and bring images to life.

At this moment, the only imlementation that exists uses DirectX9, but there will be more.

## Core functionality ##

As mentioned, the nuts and bolts lie in the `RenderingMechanism` and in a `RenderCommand`.

A renderer instance can use only one `RenderingMechanism` at a time and it manages its lifetime.
You can set it using `Renderer.setMechanism` method:

```
renderer.setMechanism( new MyRenderMechanism() );
```

The mechanism has a 3 methods in its interface:
```
class RenderingMechanism
{
public:
   virtual ~RenderingMechanism() {}

   /**
    * Initializes the mechanism with the renderer that will be using it.
    *
    * @param renderer
    */
   virtual void initialize( Renderer& renderer ) = 0;

   /**
    * Deinitializes the mechanism with the renderer that was using it.
    *
    * @param renderer
    */
   virtual void deinitialize( Renderer& renderer ) = 0;

   /**
    * Performs the rendering operation defined by the mechanism.
    *
    * @param renderer
    */
   virtual void render( Renderer& renderer ) = 0;
};
```

Each time the mechanism is set for the first time, `initialize` method will be called. And whenever it's about to be destroyed, the renderer will call the `deinitialize` method.

Each time the renderer wants the mechanism to render something, it will call its `render` command.

The parameter passed to the mechanism is the instance of the renderer.
The renderer instance, among other methods, exposes a very important one - `operator()`, which gives you access to the `RenderCommand` buffer and allows you to queue render commands:

```

void MyRenderMechanism::render( Renderer& renderer )
{
   // ...

   new ( renderer() ) new RCBindPixelShader( *m_shader );
   new ( renderer() ) new RCUnbindPixelShader( *m_shader );

   // ...
}

```

## Render commands ##

Render commands are a way of telling the underlying library what to do.
They are responsible for:
  * setting up shaders
  * drawing geoemtry
  * setting up render targets
  * retriving colors from a render surface
and many many more.

Basically, you can write your own commands you require, write their implementations in the renderer implementation library and you can start using them in your own mechanisms.

## Scene tree ##


## Geometry ##

`Geometry` is yet another entity that can be placed on a [scene tree](Renderer#Scene_tree.md). It allows to embed a resource responsible for defining the geometry along with the shaders that transform it ( [geometry resource](Renderer#Geometry_resource.md) )

Geometry on its own is just a piece of vertices connected somehow together. It can't be drawn!
You need something to paint it with - a color, a texture etc.

This is where a`RenderState` comes in. `Geometry` can manage multiple render states. Those specify how do we want to draw it.

An example of a render state is a [material entity](Renderer#Material.md) - a resource that uses [pixel shaders](Renderer#Pixel_shader.md) to define how geometry should be drawn.

`Geometry` contains two methods that allow you to define its render states:

```
/**
 * Adds a new render state.
 *
 * @param state
 */
void addState( RenderState& state );

/**
 * Removes a render state.
 *
 * @param state
 */
void removeState( RenderState& state );
```

So if you have access to a `Geometry` entity, you can just call them and set the render state of your own.
However - you can also rely on the `Geometry` to self-configure itself with a render state - whenever a child entity is added to the `Geometry` entity, and it implements the `RenderState` interface, it will automatically be added to the render states vector of that `Geometry` entity - neat :)

So you can place your `MaterialEntity` entities directly under the `Geometry` node ( just as you would aggregate your member fields in a class in your C++ code ), and the nodes will self-configure themselves.

**CAUTION**: Render state is a runtime thing, so it won't get serialized when the scene tree gets serialized.

## Geometry resource ##

### Vertex shaders output structure ###

```
struct VS_OUTPUT
{
    float4 Position     : POSITION;   // vertex position 
    float4 Diffuse      : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV    : TEXCOORD0;  // vertex texture coords 
    float3 Normal       : TEXCOORD1;  // vertex normal
    float3 ViewPosition : TEXCOORD2;  // vertex view position
};
```

## Materials ##

### Pixel shader ###

## Rendering pipeline ##

You can define the render targets to which the scene node will render the scene.
Simply set the "Render target id" parameter with the names of render targets you defined, separating them with semicolons (;).

If you want to render directly to the backbuffer, simply give a name that's doesn't match any existing render target defined for the pipeline ( that's why it's considered a good practice to reserve the word `BackBuffer` and use that for these occasions )



## Camera ##

## Cameras stack ##
There may be many cameras used throught a rendering frame time - we may be rendering the scene from a light perspective i.e.

For that very purpose, a cameras stack was introduced.
If you need, you can simply push a new camera onto a stack - it will immediately become an active camera, and all subsequent operations will use it.

```
Renderer::pushCamera( Camera& camera );
```

Once you're done with that camera, simply pop it from the stack

```
Renderer::popCamera();
```

The stack can never be empty - if you pop all cameras from it, the default one will be automatically pushed back to the top ( as the only element ). The reason for that is simple - the renderer MUST have a camera to render anything.

**CAUTION**:
The stack operates on the main rendering thread, so make sure NOT to access the cameras from the rendering queue tread, as it will not point to a correct camera any longer.