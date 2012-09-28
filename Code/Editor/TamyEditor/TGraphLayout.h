/// @file   TamyEditor/TGraphLayout.h
/// @brief  pipeline layout resource base
#ifndef _TGRAPH_LAYOUT_H
#define _TGRAPH_LAYOUT_H

#include "GraphLayout.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

class GraphBlock;

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
class TGraphLayout : public Resource, public GraphLayout
{
protected:
   typedef GenericFactory< ReflectionObject, GraphBlock >      BlocksFactory;
   typedef GenericFactory< GBNodeSocket, GraphBlockSocket >    SocketsFactory;

   TPipeline*                                         m_pipeline;
   BlocksFactory*                                     m_blocksFactory;
   SocketsFactory*                                    m_socketsFactory;

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   TGraphLayout( const FilePath& resourceName = FilePath() );
   ~TGraphLayout();

   /**
    * Returns a rendering pipeline the layout edits.
    */
   TPipeline& getPipeline() { return *m_pipeline; }

   // -------------------------------------------------------------------------
   // GraphLayout implementation
   // -------------------------------------------------------------------------
   GraphBlock* createNode( const SerializableReflectionType& type );
   void removeNode( ReflectionObject& node );
   void getNodesClasses( std::vector< const SerializableReflectionType* >& classes );
   bool connectNodes( ReflectionObject* sourceNode, const std::string& outputName, ReflectionObject* destinationNode, const std::string& inputName );
   void disconnectNode( ReflectionObject* sourceNode, ReflectionObject* destinationNode, const std::string& inputName );
   void breakPipelineConnections( const std::vector< GraphBlockConnection* >& connectionsToDelete ) const;

protected:
   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded( ResourcesManager& mgr );

   /**
    * Called in order to setup a blocks factory.
    *
    * @param factory
    */
   virtual void initBlocksFactory( BlocksFactory& factory ) = 0;

   /**
    * Called in order to setup a sockets factory.
    *
    * @param factory
    */
   virtual void initSocketsFactory( SocketsFactory& factory ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

#include "TGraphLayout.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _TGRAPH_LAYOUT_H
