#include "core-Renderer\MaterialInstance.h"
#include "core-Renderer\MaterialsDB.h"
#include "core-Renderer/Material.h"
#include "core-Renderer/MaterialNode.h"
#include "core-Renderer/Texture.h"
#include "core/RuntimeData.h"
#include "core-Renderer/MNPixelShader.h"
#include "core/Graph.h"
#include "core/GraphAlgorithms.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ENUM( MaterialTextures );
   ENUM_VAL( MT_DIFFUSE_1 );
   ENUM_VAL( MT_DIFFUSE_2 );
   ENUM_VAL( MT_NORMALS );
   ENUM_VAL( MT_SPECULAR );
END_ENUM();

///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( MaterialInstance, tmi, AM_BINARY );
   PROPERTY( TRefPtr< Material >, m_materialRenderer );
   PROPERTY( SurfaceProperties, m_surfaceProperties );
   PROPERTY( std::vector< TRefPtr< Texture > >, m_texture );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

MaterialInstance::MaterialInstance( const FilePath& resourceName )
   : Resource( resourceName )
   , m_index( -1 )
   , m_materialRenderer( NULL )
   , m_dataBuf( NULL )
{
   for ( unsigned int i = 0; i < MT_MAX; ++i )
   {
      m_texture.push_back( NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////

MaterialInstance::~MaterialInstance()
{
   deinitializeMaterial();
   detachListeners();

   m_materialRenderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::onResourceLoaded( ResourcesManager& mgr )
{
   __super::onResourceLoaded( mgr );

   // register the material with the materials DB
   MaterialsDB& materialsDB = MaterialsDB::getInstance();

   m_index = materialsDB.registerMaterial( *this );

   attachListeners();
   initializeMaterial();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::setMaterialRenderer( Material* materialRenderer )
{
   // deinitialize the old renderer dependencies
   deinitializeMaterial();
   detachListeners();

   // set the new material renderer
   m_materialRenderer = materialRenderer;

   // initialize it and the dependencies
   initializeMaterial();
   attachListeners();

   notify( MIO_MAT_CHANGED );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::update( Material& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::update( Material& subject, const GraphBuilderOperation& msg )
{
   if ( msg == GBO_PRE_CHANGE )
   {
      deinitializeMaterial();
   }
   else if ( msg == GBO_POST_CHANGE  )
   {
      initializeMaterial();
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::update( MaterialNode& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::update( MaterialNode& subject, const GraphBuilderNodeOperation& msg )
{
   deinitializeMaterial();
   initializeMaterial();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::initializeMaterial()
{
   if ( m_materialRenderer.isNull() )
   {
      return;
   }

   // create the material graph
   Graph< MaterialNode* > materialGraph;
   m_materialRenderer->buildReverseGraph< MNPixelShader >( materialGraph );

   std::vector< Graph< MaterialNode* >::Index > sortedNodes;
   GraphTopologicalSort( sortedNodes, materialGraph );

   // insert the nodes backwards, because the graph was built backwards, but we still want
   // to run the update from the start nodes to the end node
   int count = sortedNodes.size();
   for ( int i = count - 1; i >= 0; --i )
   {
      m_nodesQueue.push_back( materialGraph.getNode( sortedNodes[i] ) );
   }

   // create new runtime data buffer
   delete m_dataBuf;
   m_dataBuf = new RuntimeDataBuffer();

   // initialize the nodes
   count = m_nodesQueue.size();
   for ( int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->createLayout( *this );
   }

   // attach self as the nodes observer
   for ( std::vector< MaterialNode* >::const_iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      (*it)->attachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::deinitializeMaterial()
{
   // stop observing the nodes
   for ( std::vector< MaterialNode* >::const_iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      (*it)->detachObserver( *this );
   }
   m_nodesQueue.clear();

   delete m_dataBuf; m_dataBuf = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::attachListeners()
{
   if ( m_materialRenderer.isNotNull() )
   {
      m_materialRenderer->attachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::detachListeners()
{
   if ( m_materialRenderer.isNotNull() )
   {
      m_materialRenderer->detachObserver( *this );
   }
}


///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::preRender( Renderer& renderer, const MaterialEntity& entity ) const
{
   unsigned int count = m_nodesQueue.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->preRender( renderer, *this, entity );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstance::postRender( Renderer& renderer, const MaterialEntity& entity ) const
{
   unsigned int count = m_nodesQueue.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->postRender( renderer, *this, entity );
   }
}

///////////////////////////////////////////////////////////////////////////////
