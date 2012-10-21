/// @file   core-Renderer/RenderingPipelineNode.h
/// @brief  rendering pipeline node base
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilderNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipeline;
class RenderingPipelineMechanism;
class RenderingPipelineTransaction;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node.
 */
class RenderingPipelineNode : public ResourceObject, public GraphBuilderNode< RenderingPipelineNode >
{
   DECLARE_ALLOCATOR( RenderingPipelineNode, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   virtual ~RenderingPipelineNode();

   /**
    * Called in order to initialize node's runtime data layout.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   void createLayout( RenderingPipelineMechanism& host ) const;

   /**
    * Called in order to deinitialize node's runtime data layout.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   void destroyLayout( RenderingPipelineMechanism& host ) const;
   
   /**
    * Called in order to perform all rendering-related things.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   void update( RenderingPipelineMechanism& host ) const;

   /**
    * Creates an input with the specified name of the specified type.
    *
    * @param dataType      what type of data should the input marshal
    * @param name          input name
    */
   GBNodeInput< RenderingPipelineNode >* createInput( const ReflectionType& dataType, const std::string& name ) const;

   /**
    * Called when a node is added to the pipeline. It allows the node
    * to create all necessary render targets, depth buffers and other 
    * static resources it needs in order to work without bothering the user.
    *
    * Those resources however will be publicly accessible, so if the user wants,
    * he can later on go in and change them.
    *
    * If any necessary resources already exist, they will be used as they are,
    * without changing their current settings ( as other nodes might be using them as well ).
    *
    * CAUTION: if the node is removed, the resources will remain. That's because
    * there may be other nodes using them already etc. We won't be checking that
    * at this point, as it's not a big deal, but if a need comes, such a check
    * may be implemented in the future.
    *
    * @param transaction
    */
   virtual void onCreatePrerequisites( RenderingPipelineTransaction& transaction ) const {}

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onPropertyChanged( ReflectionProperty& property );

protected:

   /**
    * Called when the rendering mechanism creates a data layout for the runtime data.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void onCreateLayout( RenderingPipelineMechanism& host ) const {}

   /**
    * Called when the rendering mechanism destroys the current data layout.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void onDestroyLayout( RenderingPipelineMechanism& host ) const {}
   
   /**
    * Called in order to perform all rendering-related things the implementation is responsible for.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void onUpdate( RenderingPipelineMechanism& host ) const {}

};

///////////////////////////////////////////////////////////////////////////////
