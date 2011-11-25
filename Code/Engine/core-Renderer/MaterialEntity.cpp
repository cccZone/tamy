#include "core-Renderer/MaterialEntity.h"
#include "core-Renderer/Material.h"
#include "core-Renderer/MaterialNode.h"
#include "core/RuntimeData.h"
#include "core-Renderer/MNPixelShader.h"
#include "core/Graph.h"
#include "core/GraphAlgorithms.h"


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

   // create the material graph
   Graph< MaterialNode* > materialGraph;
   m_material->buildGraph< MNPixelShader >( materialGraph );

   std::vector< Graph< MaterialNode* >::Index > sortedNodes;
   GraphTopologicalSort( sortedNodes, materialGraph );

   for ( std::vector< Graph< MaterialNode* >::Index >::const_iterator it = sortedNodes.begin(); it != sortedNodes.end(); ++it )
   {
      m_nodesQueue.push_back( materialGraph.getNode( *it ) );
   }

   // create new runtime data buffer
   delete m_dataBuf;
   m_dataBuf = new RuntimeDataBuffer();

   // initialize the nodes
   unsigned int count = m_nodesQueue.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->createLayout( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::deinitializeMaterial()
{
   delete m_dataBuf; m_dataBuf = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPreRender( Renderer& renderer ) const
{
   unsigned int count = m_nodesQueue.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->preRender( renderer, *m_dataBuf );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPostRender( Renderer& renderer ) const
{
   unsigned int count = m_nodesQueue.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->postRender( renderer, *m_dataBuf );
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
