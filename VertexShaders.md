# Vertex shaders #

Writen in HLSL, they allow to define a mesh geometry should be deformed before it's rendered.

## Usage ##
They are instantiated using a `VertexShader` resource, and can be loaded as such.

```
FilePath vsPath( LIGHTING_SHADERS_DIR "Lights/pointLight.tvsh" );
m_vertexShader = resMgr.create< VertexShader >( vsPath, true );
```

Once you have a handle to a vertex shader, you can use it by binding it to a rendering device before you draw any geometry. That's important - the call that binds the shader should go **BEFORE** any calls to geometry drawing that's supposed to use that shader.

```
RCBindVertexShader* vsComm = new ( renderer() ) RCBindVertexShader( *m_vertexShader, renderer );
```

Once you're done, you should clean up using a dedicated render command:

```
new ( renderer() ) RCUnbindVertexShader( *m_vertexShader );
```

## Vertex shader binding command ##

The `RCBindVertexShader` command has a couple of methods that allow you to set the vertex shader constants:

```
void setBool( const IDString& paramName, bool val );
void setInt( const IDString& paramName, int val );
void setInt( const IDString& paramName, const int* arr, unsigned int size );
void setFloat( const IDString& paramName, float val );
void setFloat( const IDString& paramName, const float* arr, unsigned int size );
void setMtx( const IDString& paramName, const Matrix& val );
void setMtx( const IDString& paramName, const Matrix* arr, unsigned int size );
void setString( const IDString& paramName, const std::string& val );
void setTexture( const IDString& paramName, ShaderTexture* val );
void setVec4( const IDString& paramName, const Vector& val );
void setVec4( const IDString& paramName, const Vector* arr, unsigned int size );
```

Each constant name is identified using an [IDString](IDString.md) for performance reasons.

## Support for multiple rendering techniques ##

Imagine that you're  rendering an arbitrary mesh. The mesh represents a gello, and you want it to shake with every movement.

What you would probably do is write a custom vertex shader that would apply some wavy motion to particular groups of vertices, making the mesh appear as if it was shaking, even though the underlying geometry is perfectly still.

But now imagine that you came up with a custom shader that calculates per pixel motion blur, or some facy lighting. In order to have it, you need to expand your vertex shader with some extra capabilities, so that it calculates extra data.

That additional requirement would add new code to your existing shader, mixing two functionalities you may not even want ( say your lighting shader doesn't caer that much about the shaky geometry and can safely do without it ).


Another thing to keep in mind is that this requirement for a completely new vertex shading, or data it outputs, comes from a dedicated pixel shader - which is operating on data the vertex shader outputs.


In order to support cases like this, where we have a need for different vertex shading techniques, the engine introduces a concept of a Vertex shader technique.

### HLSL ###

A technique is just a regular shader function , treated as a **main** function ( it needs to have an output semantics set etc. ).


You mark such a technique using a special annotation placed directly above the function declaration:
```
// <technique>
VS_SHADOW_OUTPUT mainShadow( VDI_SIMPLE In )
{
   // ...
}
```

The function must return a structure, because its name is used as a definition of the techique name and assures, that the pixel shader will now which VS technique to select.

On the other hand, a pixel shader that requires that technique can now be annotated with a reference to it, and should use the same input structure the vertex shader function returned.
```
// <psMain>
float4 main( VS_SHADOW_OUTPUT In ) : COLOR0 {
   // ...
}
```

### C++ ###
A technique may require some extra data constants being set on the C++ side.

A `VertexShaderConfigurator` interface serves just that very purpose.

It has just one method
```
void configure( const Geometry& geometry, RCBindVertexShader* command );
```

It's a Visitor pattern and will be called for every rendered `Geometry` entity rendered from a context of a particular vertex shader command.

The method is a place to setup additional shader parameters required by the particular vertex shader technique.

You pass an instance to it to the rendered geometry, like so:
```
   VSSetter vsSetter;  // <-- a VertexShaderConfigurator implementation
   someGometry->render( renderer, &vsSetter );
```


### What technique will be selected for rendering ###
But how do you know which technique will be used and what constants you should set?

Well - at this moment you can easily trace it from the code flow.
The current pipeline of working with the shaders is:
1. bind a pixel shader
2. bind a vertex shader
3. render the geometry
4. unbind the vertex shader
5. unbind the pixel shader

That gives you a fairly accurate knowledge which vertex shader technique  will be used at point 3, since the employed pixel shader will define it.

I must mention that a pixel shader can have only one entry point, and thus it has a unique vertex shader technique assigned to it.

### Possible refactoring ###
If the need comes, I might extend the interfaces a bit to inform which technique will be used, so that we can create universal setters for a group of vertex shaders that have a `switch` clause inside or something and configure the shaders that way -  but for now, there's no need for that.