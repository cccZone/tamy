/// @file   TamyEditor/PipelineLayout.h
/// @brief  pipeline layout resource base
#ifndef _PIPELINE_LAYOUT_H
#define _PIPELINE_LAYOUT_H

#include "GraphLayout.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

class PipelineBlock;

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
class PipelineLayout : public Resource, public GraphLayout, public GenericFactory< TNode, PipelineBlock >
{
protected:
   TPipeline*                  m_pipeline;

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   PipelineLayout( const FilePath& resourceName = FilePath() );
   ~PipelineLayout();

   /**
    * Returns a rendering pipeline the layout edits.
    */
   TPipeline& getPipeline() { return *m_pipeline; }

   // -------------------------------------------------------------------------
   // GraphLayout implementation
   // -------------------------------------------------------------------------
   GraphBlock* createNode( const Class& type );
   void removeNode( Object& node );
   void getNodesClasses( std::vector< Class >& classes );

protected:
   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded( ResourcesManager& mgr );

   /**
    * Called when we need to set a block up with socket definitions.
    *
    * @param block
    */
   virtual void initSocketsFactory( PipelineBlock& block ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

#include "PipelineLayout.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _PIPELINE_LAYOUT_H
