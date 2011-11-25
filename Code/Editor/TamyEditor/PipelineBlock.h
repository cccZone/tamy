/// @file   TamyEditor/PipelineBlock.h
/// @brief  pipeline block representation base
#ifndef _PIPELINE_BLOCK_H
#define _PIPELINE_BLOCK_H

#include "GraphBlock.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

class PipelineBlock : public GraphBlock
{
protected:
   GenericFactory< GBNodeSocket, GraphBlockSocket >*     m_socketsFactory;

public:
   virtual ~PipelineBlock();

   /**
    * Called in order to initialize the block
    */
   virtual void initialize() = 0;

   /**
    * Returns the sockets factory.
    */
   inline GenericFactory< GBNodeSocket, GraphBlockSocket >& getSocketsFactory() const { return *m_socketsFactory; }

protected:
   PipelineBlock();
};

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
class TPipelineBlock : public PipelineBlock, public Observer< typename TBaseNode, GraphBuilderNodeOperation >
{
protected:
   TNode*                                                m_nodePtr;
   TResourceHandle< typename TNode >*                    m_node;
   
public:
   /**
    * Default constructor.
    */
   TPipelineBlock();

   /**
    * Constructor.
    *
    * @param node       represented node
    */
   TPipelineBlock( TNode& node );
   virtual ~TPipelineBlock();

   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   void initialize();
   Object& getNode();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update( TBaseNode& subject );
   void update( TBaseNode& subject, const GraphBuilderNodeOperation& msg );

protected:
   /**
    * Refreshes the block's inputs.
    */
   void refreshInputs( TBaseNode& node );

   /**
    * Refreshes the block's outputs.
    */
   void refreshOutputs( TBaseNode& node );
   
private:
   void initSocketsFactory();
};

///////////////////////////////////////////////////////////////////////////////

#include "PipelineBlock.inl"

///////////////////////////////////////////////////////////////////////////////

#endif _PIPELINE_BLOCK_H
