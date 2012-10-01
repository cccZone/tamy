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
   uint count = m_blocks.size();
   for ( uint i = 0; i < count; ++i )
   {
      TNode* node = static_cast< TNode* >( m_blocks[i]->getNode() );
      node->detachObserver( *this );
   }

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

   // go through the existing blocks and set up their the sockets factory,
   // and attach self as an observer of their nodes
   count = (int)m_blocks.size();
   for ( int i = 0; i < count; ++i )
   {
      GraphBlock* block = m_blocks[i];
      block->setSocketsFactory( m_socketsFactory );
      
      TNode* node = static_cast< TNode* >( block->getNode() );
      node->attachObserver( *this );
   }

   restoreState();
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::update( TNode& node )
{
   GraphBlock* block = getNodeBlock( node );
   refreshInputs( node, block );
   refreshOutputs( node, block );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::update( TNode& node, const GraphBuilderNodeOperation& msg )
{
   // locate a block that corresponds to the node that's been changed
   if ( msg == GBNO_INPUTS_CHANGED )
   {
      GraphBlock* block = getNodeBlock( node );
      refreshInputs( node, block );
   }
   else if ( msg == GBNO_OUTPUTS_CHANGED )
   {
      GraphBlock* block = getNodeBlock( node );
      refreshOutputs( node, block );
   }
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

   // attach self as an observer to the node
   refreshInputs( *node, block );
   refreshOutputs( *node, block );
   node->attachObserver( *this, false );

   return block;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::removeNode( ReflectionObject& node )
{
   TNode* rpNode = dynamic_cast< TNode* >( &node );
   ASSERT_MSG( rpNode != NULL, "Attempting to remove a node that's not a GraphBuilderNode" );

   // detach self as an observer to the node
   rpNode->detachObserver( *this );

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

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::refreshInputs( TNode& node, GraphBlock* block )
{
   // first - gather the names of all inputs the block currently has
   std::set< std::string > inputNames;
   block->getAllSockets( GBSP_INPUT, inputNames );

   // now go through the node's inputs and see which ones were just created
   const std::vector< GBNodeInput< TNode >* >& inputs = node.getInputs();
   for ( std::vector< GBNodeInput< TNode >* >::const_iterator it = inputs.begin(); it != inputs.end(); ++it )
   {
      // check if an input with this name doesn't already exist - if it does, don't create one
      const std::string& inputName = (*it)->getName();
      if ( inputNames.find( inputName ) == inputNames.end() )
      {
         GraphBlockSocket* socket = m_socketsFactory->create( **it );
         if ( !socket )
         {
            continue;
         }
         socket->initialize( block, GBSP_INPUT, inputName.c_str() );
         block->addSocket( socket );
      }

      inputNames.erase( inputName );
   }

   // remove broken connections
   removeBrokenConnections( block, inputNames );

   // now go through the remaining input names - those are the inputs that were deleted - and remove them
   block->removeSockets( GBSP_INPUT, inputNames );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::refreshOutputs( TNode& node, GraphBlock* block )
{
   // first - gather the names of all outputs the block currently has
   std::set< std::string > outputNames;
   block->getAllSockets( GBSP_OUTPUT, outputNames );

   // now go through the node's outputs and see which ones were just created
   const std::vector< GBNodeOutput< TNode >* >& outputs = node.getOutputs();
   for ( std::vector< GBNodeOutput< TNode >* >::const_iterator it = outputs.begin(); it != outputs.end(); ++it )
   {
      // check if an input with this name doesn't already exist - if it does, don't create one
      const std::string& outputName = (*it)->getName();
      if ( outputNames.find( outputName ) == outputNames.end() )
      {
         GraphBlockSocket* socket = m_socketsFactory->create( **it );
         if ( !socket )
         {
            continue;
         }
         socket->initialize( block, GBSP_OUTPUT, outputName.c_str() );
         block->addSocket( socket );
      }

      outputNames.erase( outputName );
   }

   // remove broken connections
   removeBrokenConnections( block, outputNames );

   // now go through the remaining output names - those are the inputs that were deleted - and remove them
   block->removeSockets( GBSP_OUTPUT, outputNames );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
GraphBlock* TGraphLayout< TPipeline, TNode >::getNodeBlock( const TNode& node ) const
{
   GraphBlock* block = NULL;
   uint count = m_blocks.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_blocks[i]->getNode() == & node )
      {
         return m_blocks[i];
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void TGraphLayout< TPipeline, TNode >::removeBrokenConnections( const GraphBlock* block, const std::set< std::string >& removedSocketsNames )
{
   int count = m_connections.size();
   std::set< GraphBlockConnection* > brokenConnections;
   for ( int i = count - 1; i >= 0; --i )
   {
      GraphBlockConnection* connection = m_connections[i];
      GraphBlockSocket& sourceSocket = connection->getSource();
      GraphBlockSocket& destinationSocket = connection->getDestination();
      if ( sourceSocket.getParentBlock() == block )
      {
         std::set< std::string >::const_iterator nameIt = removedSocketsNames.find( sourceSocket.getName() );
         if ( nameIt != removedSocketsNames.end() )
         {
            brokenConnections.insert( connection );
            m_connections.erase( m_connections.begin() + i );
         }
      }
      else  if ( destinationSocket.getParentBlock() == block )
      {
         std::set< std::string >::const_iterator nameIt = removedSocketsNames.find( destinationSocket.getName() );
         if ( nameIt != removedSocketsNames.end() )
         {
            brokenConnections.insert( connection );
            m_connections.erase( m_connections.begin() + i );
         }
      }
   }

   for ( std::set< GraphBlockConnection* >::const_iterator it = brokenConnections.begin(); it != brokenConnections.end(); ++it )
   {
      removeItem( *it );
      delete *it;
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _TGRAPH_LAYOUT_H
