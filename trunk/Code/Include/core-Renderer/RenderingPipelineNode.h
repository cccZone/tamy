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
class RenderingPipelineNode : public ResourceObject, public GraphBuilderNode< RenderingPipelineNode >
{
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

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
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
