#include "core-Renderer/MaterialEntity.h"
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
END_ENUM();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MaterialEntity );
   PARENT( Entity );
   PROPERTY_EDIT( "material", Material*, m_material );
   PROPERTY_EDIT( "diffuse tex 1", Texture*, m_texture[ MT_DIFFUSE_1 ] );
   PROPERTY_EDIT( "diffuse tex 2", Texture*, m_texture[ MT_DIFFUSE_2 ] );
   PROPERTY_EDIT( "normals tex", Texture*, m_texture[ MT_NORMALS ] );
   PROPERTY_EDIT( "surface properties", SurfaceProperties, m_surfaceProperties );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MaterialEntity::MaterialEntity( const std::string& name )
   : Entity( name )
   , m_material( NULL )
   , m_dataBuf( NULL )
{
   for ( unsigned int i = 0; i < MT_MAX; ++i )
   {
      m_texture[i] = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

MaterialEntity::~MaterialEntity()
{
   deinitializeMaterial();
   detachListeners();
   m_material = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onObjectLoaded()
{
   __super::onObjectLoaded();

   attachListeners();
   initializeMaterial();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPrePropertyChanged( ReflectionProperty& property )
{
   __super::onPrePropertyChanged( property );

   if ( property.getName() == "m_material" )
   {
      deinitializeMaterial();
      detachListeners();
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_material" )
   {
      initializeMaterial();
      attachListeners();
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::update( Material& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::update( Material& subject, const GraphBuilderOperation& msg )
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

void MaterialEntity::update( MaterialNode& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::update( MaterialNode& subject, const GraphBuilderNodeOperation& msg )
{
   deinitializeMaterial();
   initializeMaterial();
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

   // start observing nodes
   for ( std::vector< MaterialNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      (*it)->attachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::deinitializeMaterial()
{
   // stop observing nodes
   for ( std::vector< MaterialNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      (*it)->detachObserver( *this );
   }

   m_nodesQueue.clear();

   delete m_dataBuf; m_dataBuf = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::attachListeners()
{
   if ( m_material )
   {
       m_material->attachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::detachListeners()
{
   if ( m_material )
   {
      m_material->detachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPreRender( Renderer& renderer ) const
{
   unsigned int count = m_nodesQueue.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->preRender( renderer, *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::onPostRender( Renderer& renderer ) const
{
   unsigned int count = m_nodesQueue.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->postRender( renderer, *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialEntity::onEquals( const MaterialEntity& rhs ) const
{
   if ( m_material != rhs.m_material )
   {
      return false;
   }

   for ( unsigned int i = 0; i < MT_MAX; ++i )
   {
      if ( m_texture[i] != rhs.m_texture[i] )
      {
         return false;
      }
   }

   return m_surfaceProperties == rhs.m_surfaceProperties;
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialEntity::onLess( const MaterialEntity& rhs ) const
{
   if ( m_material < rhs.m_material )
   {
      return true;
   }

   for ( unsigned int i = 0; i < MT_MAX; ++i )
   {
      if ( m_texture[i] < rhs.m_texture[i] )
      {
         return true;
      }
   }

   return ( &m_surfaceProperties < &rhs.m_surfaceProperties );
}

///////////////////////////////////////////////////////////////////////////////

Entity* MaterialEntity::cloneSelf() const
{
   return new MaterialEntity( *this );
}

///////////////////////////////////////////////////////////////////////////////
