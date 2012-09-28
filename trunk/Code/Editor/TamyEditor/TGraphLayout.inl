#ifndef _TGRAPH_LAYOUT_H
#error "This file can only be included from TGraphLayout.h"
#else

#include "GraphBlock.h"
#include <QBrush>
#include <QImage>
#include "core/ResourcesManager.h"
#include "core/Filesystem.h"


///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
TGraphLayout< TPipeline, TNode >::TGraphLayout( const FilePath& resourceName )
   : Resource( resourceName )
   , m_pipeline( NULL )
{
   const std::string& rootDir = ResourcesManager::getInstance().getFilesystem().getCurrRoot();
   std::string imagePath = rootDir + "Editor/Icons/pipelineEditorBackground.png";
   QImage image( imagePath.c_str() );
   QBrush bgImageBrush( image );
   setBackgroundBrush( bgImageBrush );

   m_blocksFactory = new BlocksFactory();
   m_socketsFactory = new SocketsFactory();
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
TGraphLayout< TPipeline, TNode >::~TGraphLayout()
{
   delete m_blocksFactory;
   m_blocksFactory = NULL;

   delete m_socketsFactory;
   m_socketsFactory = NULL;
}


///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::onResourceLoaded( ResourcesManager& mgr ) 
{
   if ( m_pipeline == NULL )
   {
      // the model was not set - create an instance
      std::string resourcePath = getFilePath();
      m_pipeline = mgr.findResource< TPipeline >( resourcePath );
      if ( m_pipeline == NULL )
      {

         m_pipeline = new TPipeline( resourcePath );
         mgr.addResource( m_pipeline );
      }
   }

   // remove non-existing blocks
   int count = (int)m_blocks.size();
   for ( int i = count - 1; i >= 0; --i )
   {
      if ( m_blocks[i] == NULL )
      {
         m_blocks.erase( m_blocks.begin() + i );
      }
   }

   // initialize factories
   initBlocksFactory( *m_blocksFactory );
   initSocketsFactory( *m_socketsFactory );

   // go through the existing blocks and set up their the sockets factory
   count = (int)m_blocks.size();
   for ( int i = 0; i < count; ++i )
   {
      m_blocks[i]->setSocketsFactory( m_socketsFactory );
   }

   restoreState();
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
GraphBlock* TGraphLayout< TPipeline, TNode >::createNode( const SerializableReflectionType& type )
{
   TNode* node = type.instantiate< TNode >();
   GraphBlock* block = m_blocksFactory->create( *node );
   block->setSocketsFactory( m_socketsFactory );

   GraphBuilderTransaction< TPipeline, TNode >* transaction = TPipeline::createTransaction( *m_pipeline );
   transaction->addNode( node );
   transaction->commit();
   delete transaction;

   block->initialize();

   return block;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::removeNode( ReflectionObject& node )
{
   TNode* rpNode = dynamic_cast< TNode* >( &node );
   ASSERT_MSG( rpNode != NULL, "Attempting to remove a node that's not a GraphBuilderNode" );

   GraphBuilderTransaction< TPipeline, TNode >* transaction = TPipeline::createTransaction( *m_pipeline );
   transaction->removeNode( *rpNode );
   transaction->commit();
   delete transaction;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::getNodesClasses( std::vector< const SerializableReflectionType* >& classes )
{
   ReflectionTypesRegistry& classesReg = ReflectionTypesRegistry::getInstance();
   classesReg.getMatchingSerializableTypes< TNode >( classes, false );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
bool TGraphLayout< TPipeline, TNode >::connectNodes( ReflectionObject* sourceNode, const std::string& outputName, ReflectionObject* destinationNode, const std::string& inputName )
{
   GraphBuilderTransaction< TPipeline, TNode >* transaction = TPipeline::createTransaction( *m_pipeline );
   bool result = transaction->connectNodes( static_cast< TNode* >( sourceNode ), outputName, static_cast< TNode* >( destinationNode ), inputName );
   transaction->commit();
   delete transaction;

   return result;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::disconnectNode( ReflectionObject* sourceNode, ReflectionObject* destinationNode, const std::string& inputName )
{
   GraphBuilderTransaction< TPipeline, TNode >* transaction = TPipeline::createTransaction( *m_pipeline );
   transaction->disconnectNodes( static_cast< TNode* >( sourceNode ), static_cast< TNode* >( destinationNode ), inputName );
   transaction->commit();
   delete transaction;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::breakPipelineConnections( const std::vector< GraphBlockConnection* >& connectionsToDelete ) const
{
   GraphBuilderTransaction< TPipeline, TNode >* transaction = TPipeline::createTransaction( *m_pipeline );

   uint count = connectionsToDelete.size();
   for ( uint i = 0; i < count; ++i )
   {
      GraphBlockConnection* connection = connectionsToDelete[i];
      
      GraphBlockSocket& sourceSocket = connection->getSource();
      GraphBlockSocket& destSocket = connection->getDestination();
      ReflectionObject* sourceNode = sourceSocket.getParentBlock()->getNode();
      ReflectionObject* destNode = destSocket.getParentBlock()->getNode();
      const std::string& inputName = destSocket.getName();
      transaction->disconnectNodes( static_cast< TNode* >( sourceNode ), static_cast< TNode* >( destNode ), inputName );
   }

   transaction->commit();
   delete transaction;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _TGRAPH_LAYOUT_H
