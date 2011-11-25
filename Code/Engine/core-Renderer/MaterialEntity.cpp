#include "core-Renderer/MaterialEntity.h"
#include "core-Renderer/Material.h"
#include "core/RuntimeData.h"
#include "core-Renderer/MNPixelShader.h"


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
   if ( !m_material )
   {
      return;
   }
   /*
   // create the material graph
   Graph< MaterialNode* > materialGraph;
   m_material->buildGraph< MNPixelShader >( materialGraph );

   std::vector< Graph< MaterialNode* >::Index > sortedNodes;
   GraphTopologicalSort( sortedNodes, graph );

   for ( std::vector< Graph< MaterialNode* >::Index >::const_iterator it = sortedNodes.begin(); it != sortedNodes.end(); ++it )
   {
      m_nodesQueue.push_back( graph.getNode( *it ) );
   }

   m_dataBuf = new RuntimeDataBuffer();

   // initialize the nodes
   */
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::deinitializeMaterial()
{
   ASSERT( m_dataBuf != NULL );

   delete m_dataBuf; m_dataBuf = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPreRender( Renderer& renderer, RuntimeDataBuffer& data ) const
{
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPostRender( Renderer& renderer, RuntimeDataBuffer& data ) const
{
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
