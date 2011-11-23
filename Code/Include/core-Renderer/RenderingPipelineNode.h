/// @file   core-Renderer/RenderingPipelineNode.h
/// @brief  rendering pipeline node base
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilderNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineMechanism;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node.
 */
class RenderingPipelineNode : public ResourceObject, public GraphBuilderNode< RenderingPipelineNode, RPNodeInput, RPNodeOutput >
{
   DECLARE_CLASS( RenderingPipelineNode )

public:
   virtual ~RenderingPipelineNode();

   /**
    * Called in order to initialize node's runtime data layout.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   void createLayout( RenderingPipelineMechanism& host ) const;

   /**
    * Called in order to initialize the node. This takes place before the pipeline
    * is first use in the rendering process.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   void initialize( RenderingPipelineMechanism& host ) const;

   /**
    * Called in order to initialize the node. This takes place once
    * the entire pipeline stops being used for rendering and gets deinitialized.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   void deinitialize( RenderingPipelineMechanism& host ) const;
   
   /**
    * Called in order to perform all rendering-related things.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   void update( RenderingPipelineMechanism& host ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPropertyChanged( Property& property );

protected:
   /**
    * Called when the rendering mechanism creates a data layout for the runtime data.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void onCreateLayout( RenderingPipelineMechanism& host ) const {}

   /**
    * Called in order to initialize the node implementation. This takes place before the pipeline
    * is first use in the rendering process.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void onInitialize( RenderingPipelineMechanism& host ) const {}

   /**
    * Called in order to initialize the node implementation. This takes place once
    * the entire pipeline stops being used for rendering and gets deinitialized.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void onDeinitialize( RenderingPipelineMechanism& host ) const {}
   
   /**
    * Called in order to perform all rendering-related things the implementation is responsible for.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void onUpdate( RenderingPipelineMechanism& host ) const {}

};

///////////////////////////////////////////////////////////////////////////////
