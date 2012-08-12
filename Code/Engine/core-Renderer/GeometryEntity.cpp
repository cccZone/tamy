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
   , m_vsNode( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

GeometryEntity::~GeometryEntity()
{
   deinitializeGeometryShader();
   detachListeners();
   m_geometryShader = NULL;
   m_vsNode = NULL;
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

   // create the geometry graph
   Graph< GeometryShaderNode* > geometryGraph;
   m_geometryShader->buildReverseGraph< GNVertexShader >( geometryGraph );

   std::vector< Graph< GeometryShaderNode* >::Index > sortedNodes;
   GraphTopologicalSort( sortedNodes, geometryGraph );

   // insert the nodes backwards, because the graph was built backwards, but we still want
   // to run the update from the start nodes to the end node
   int count = sortedNodes.size();
   for ( int i = count - 1; i >= 0; --i )
   {
      m_nodesQueue.push_back( geometryGraph.getNode( sortedNodes[i] ) );
   }

   // get the vertex shader node instance
   if ( sortedNodes.empty() )
   {
      m_vsNode = NULL;
   }
   else
   {
      m_vsNode = static_cast< GNVertexShader* >( m_nodesQueue.back() );
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

RCBindVertexShader* GeometryEntity::onPreRender( Renderer& renderer )
{
   unsigned int count = m_nodesQueue.size();
   if ( count == 0 || m_vsNode == NULL )
   {
      return NULL;
   }

   // make the nodes prepare all data for rendering
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->preRender( renderer, *this );
   }

   // finally set up the render command
   RCBindVertexShader* comm = m_vsNode->createRenderCommand( renderer, *this );

   return comm;
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
