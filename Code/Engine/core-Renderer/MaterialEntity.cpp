#include "core-Renderer/MaterialEntity.h"
#include "core-Renderer/Material.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MaterialEntity, Entity )
   PROPERTY_EDIT( "material", Material*, m_material )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

MaterialEntity::MaterialEntity( const std::string& name )
   : Entity( name )
   , m_material( NULL )
   , m_dataBuf( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialEntity::~MaterialEntity()
{
   deinitializeMaterial();
   m_material = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onObjectLoaded()
{
   __super::onObjectLoaded();

   initializeMaterial();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPrePropertyChanged( Property& property )
{
   __super::onPrePropertyChanged( property );

   if ( property.getName() == "m_material" )
   {
      deinitializeMaterial();
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPropertyChanged( Property& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_material" )
   {
      initializeMaterial();
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::initializeMaterial()
{
   ASSERT( m_dataBuf == NULL );

   m_dataBuf = new RuntimeDataBuffer();

   if ( m_material )
   {
      m_material->initialize( *this, *m_dataBuf );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::deinitializeMaterial()
{
   ASSERT( m_dataBuf != NULL );

   if ( m_material )
   {
      m_material->deinitialize( *this, *m_dataBuf );
   }

   delete m_dataBuf; m_dataBuf = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPreRender( Renderer& renderer, RuntimeDataBuffer& data ) const
{
   if ( m_material )
   {
      m_material->onPreRender( renderer, *m_dataBuf );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPostRender( Renderer& renderer, RuntimeDataBuffer& data ) const
{
   if ( m_material )
   {
      m_material->onPreRender( renderer, *m_dataBuf );
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
