/// @file   TamyEditor/RenderingPipelineBlock.h
/// @brief  rendering pipeline block representation base
#ifndef _RENDERING_PIPELINE_BLOCK_H
#define _RENDERING_PIPELINE_BLOCK_H

#include "GraphBlock.h"
#include "core-Renderer.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipeline;

///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineBlock : public GraphBlock
{
   DECLARE_CLASS( RenderingPipelineBlock )

private:
   GenericFactory< RPNodeSocket, GraphBlockSocket >*      m_socketsFactory;

public:
   /**
    * Default constructor.
    */
   RenderingPipelineBlock();

   /**
    * Constructor.
    *
    * @param node       represented node
    */
   RenderingPipelineBlock( RenderingPipelineNode& node );
   virtual ~RenderingPipelineBlock();

   /**
    * Called in order to initialize the block
    *
    * @param parentResource      resource that manages the object the block represents
    */
   virtual void initialize( RenderingPipeline& parentResource ) {}

protected:
   /**
    * Refreshes the block's inputs.
    */
   void refreshInputs( RenderingPipelineNode& node );

   /**
    * Refreshes the block's outputs.
    */
   void refreshOutputs( RenderingPipelineNode& node );
   
private:
   void initSocketsFactory();
};

///////////////////////////////////////////////////////////////////////////////

template< typename T >
class TRenderingPipelineBlock : public RenderingPipelineBlock, public Observer< RenderingPipelineNode, RenderingPipelineNodeOperation >
{
protected:
   T*                                 m_nodePtr;
   TResourceHandle< T >*              m_node;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   TRenderingPipelineBlock() : RenderingPipelineBlock(), m_nodePtr( NULL ), m_node( NULL ) {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   TRenderingPipelineBlock( T& node );
   virtual ~TRenderingPipelineBlock();

   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   Object& getNode();
   void initialize( RenderingPipeline& parentResource );

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update( RenderingPipelineNode& subject );
   void update( RenderingPipelineNode& subject, const RenderingPipelineNodeOperation& msg );
};

///////////////////////////////////////////////////////////////////////////////

#include "RenderingPipelineBlock.inl"

///////////////////////////////////////////////////////////////////////////////

#endif _RENDERING_PIPELINE_BLOCK_H
