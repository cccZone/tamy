#include "core-Renderer/MaterialEntity.h"
#include "core-Renderer/MaterialInstance.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MaterialEntity );
   PARENT( RenderState );
   PROPERTY_EDIT( "Material instance", MaterialInstance*, m_material );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MaterialEntity::MaterialEntity( const std::string& name )
   : TRenderState< MaterialEntity >( name )
   , m_material( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialEntity::MaterialEntity( const MaterialEntity& rhs )
   : TRenderState< MaterialEntity >( rhs )
   , m_material( rhs.m_material )
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialEntity::~MaterialEntity()
{
   m_material = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::setMaterial( MaterialInstance* materialInstance )
{
   m_material = materialInstance;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPreRender( Renderer& renderer ) const
{
   if ( m_material )
   {
      m_material->preRender( renderer, *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPostRender( Renderer& renderer ) const
{
   if ( m_material )
   {
      m_material->postRender( renderer, *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialEntity::onEquals( const MaterialEntity& rhs ) const
{
   return m_material == rhs.m_material;
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialEntity::onLess( const MaterialEntity& rhs ) const
{
   return m_material < rhs.m_material;
}

///////////////////////////////////////////////////////////////////////////////

Entity* MaterialEntity::cloneSelf() const
{
   return new MaterialEntity( *this );
}

///////////////////////////////////////////////////////////////////////////////
