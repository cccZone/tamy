#include "core-Renderer/Material.h"
#include "core-Renderer/MaterialEntity.h"
#include "core-Renderer/MaterialNode.h"
#include "core-Renderer/Renderer.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( Material, Resource, tmat, AM_BINARY )
   PROPERTY( std::vector< MaterialNode* >, m_nodes );
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

Material::Material( const FilePath& resourceName )
   : Resource( resourceName )
{
}

///////////////////////////////////////////////////////////////////////////////

Material::~Material()
{
}

///////////////////////////////////////////////////////////////////////////////

void Material::onNodeAdded( MaterialNode* node )
{
   addObject( node );
}

///////////////////////////////////////////////////////////////////////////////

void Material::onNodeRemoved( MaterialNode& node )
{
   removeObject( node.getObjectId() );
}

///////////////////////////////////////////////////////////////////////////////
