#include "core-Renderer/DirectionalLight.h"


///////////////////////////////////////////////////////////////////////////////

void RCRenderDirectionalLight::execute( Renderer& renderer )
{
   // <lighting.todo> !!!! IMPLEMENT ME
   // this thing should a viewport-aligned quad that fills the entire viewport
   // and affects every pixel on the screen, that passes the Z-Buffer test


   // <lighting.todo>
   // 1. rendering the scene to a G-Buffer - we have the computation mode in the scene render node,
   // but it simply renders the whole geometry using a different shader ( one shader ).
   // What we want is an ability to render the geometry using the shaders defined in their
   // materials, but to multiple buffers that will contain different attributes ( color, depth, normals etc. )
   //
   // 2. ability to display the contents of those buffers for debug purposes ( a simple post-process node
   // with a simple copy shader will do )
   //
   // 3. the deferred lighting shader
   //
   // 4. this lighting shape renderer ( and one for each remaining light type as well )


}

///////////////////////////////////////////////////////////////////////////////
