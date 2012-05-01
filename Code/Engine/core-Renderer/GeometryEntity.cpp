#include "core-Renderer/GeometryEntity.h"
#include "core-Renderer/GeometryShader.h"
#include "core-Renderer/GeometryShaderNode.h"
#include "core/RuntimeData.h"
#include "core-Renderer/GNVertexShader.h"
#include "core/Graph.h"
#include "core/GraphAlgorithms.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GeometryEntity );
   PARENT( Geometry );
   PROPERTY_EDIT( "geometry shader", GeometryShader*, m_geometryShader );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GeometryEntity::GeometryEntity( const std::string& name )
   : Geometry( name )
   , m_geometryShader( NULL )
   , m_dataBuf( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

GeometryEntity::~GeometryEntity()
{
   deinitializeGeometryShader();
   detachListeners();
   m_geometryShader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::onObjectLoaded()
{
   __super::onObjectLoaded();

   attachListeners();
   initializeGeometryShader();
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::onPrePropertyChanged( ReflectionProperty& property )
{
   __super::onPrePropertyChanged( property );

   if ( property.getName() == "m_geometryShader" )
   {
      deinitializeGeometryShader();
      detachListeners();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_geometryShader" )
   {
      initializeGeometryShader();
      attachListeners();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::update( GeometryShader& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::update( GeometryShader& subject, const GraphBuilderOperation& msg )
{
   if ( msg == GBO_PRE_CHANGE )
   {
      deinitializeGeometryShader();
   }
   else if ( msg == GBO_POST_CHANGE  )
   {
      initializeGeometryShader();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::update( GeometryShaderNode& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::update( GeometryShaderNode& subject, const GraphBuilderNodeOperation& msg )
{
   deinitializeGeometryShader();
   initializeGeometryShader();
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::initializeGeometryShader()
{
   if ( !m_geometryShader )
   {
      return;
   }

   // create the material graph
   Graph< GeometryShaderNode* > materialGraph;
   m_geometryShader->buildGraph< GNVertexShader >( materialGraph );

   std::vector< Graph< GeometryShaderNode* >::Index > sortedNodes;
   GraphTopologicalSort( sortedNodes, materialGraph );

   for ( std::vector< Graph< GeometryShaderNode* >::Index >::const_iterator it = sortedNodes.begin(); it != sortedNodes.end(); ++it )
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
   for ( std::vector< GeometryShaderNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      (*it)->attachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::deinitializeGeometryShader()
{
   // stop observing nodes
   for ( std::vector< GeometryShaderNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      (*it)->detachObserver( *this );
   }

   m_nodesQueue.clear();

   delete m_dataBuf; m_dataBuf = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::attachListeners()
{
   if ( m_geometryShader )
   {
      m_geometryShader->attachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::detachListeners()
{
   if ( m_geometryShader )
   {
      m_geometryShader->detachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool GeometryEntity::onPreRender( Renderer& renderer )
{
   unsigned int count = m_nodesQueue.size();
   if ( count == 0 )
   {
      return false;
   }

   for ( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->preRender( renderer, *this );
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

void GeometryEntity::onPostRender( Renderer& renderer )
{
   unsigned int count = m_nodesQueue.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->postRender( renderer, *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

Entity* GeometryEntity::cloneSelf() const
{
   return new GeometryEntity( *this );
}

///////////////////////////////////////////////////////////////////////////////
