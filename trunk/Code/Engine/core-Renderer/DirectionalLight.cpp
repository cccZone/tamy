#include "core-Renderer\DirectionalLight.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core\BoundingSpace.h"
#include "core-Renderer\Renderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( DirectionalLight );
   PARENT( Light );
   PROPERTY_EDIT( "Ambient color", Color, m_ambient );
   PROPERTY_EDIT( "Diffuse color", Color, m_diffuse );
   PROPERTY_EDIT( "Specular color", Color, m_specular );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::DirectionalLight( const std::string& name )
   : Light( name )
   , m_ambient(0, 0, 0, 0)
   , m_diffuse(0, 0, 0, 0)
   , m_specular(0, 0, 0, 0)
{
   setBoundingVolume( new BoundingSpace() ); 
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::onAccept( NodeVisitor& visitor )
{
   REGISTER_NODE_VISITOR( TNodesVisitor<DirectionalLight> );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::render( Renderer& renderer )
{
   new ( renderer() ) RCRenderDirectionalLight( *this );
}

///////////////////////////////////////////////////////////////////////////////
