/// @file   core-Renderer/RenderingPipelineNode.h
/// @brief  rendering pipeline node base
#pragma once

#include "core/Resource.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineMechanism;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node.
 *
 * A node keeps track of the nodes it's connected to ( for speed access
 * reasons ), however the connections are managed entirely by
 * the algorithms in the RenderingPipeline class - thus the friendship
 * with this class.
 */
class RenderingPipelineNode : public ResourceObject
{
   DECLARE_CLASS( RenderingPipelineNode )

private:
   std::vector< RenderingPipelineNode* >           m_neighbors;

public:
   virtual ~RenderingPipelineNode() {}

   /**
    * Called in order to initialize the node. This takes place before the pipeline
    * is first use in the rendering process.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void initialize( RenderingPipelineMechanism& host ) {}

   /**
    * Called in order to initialize the node. This takes place once
    * the entire pipeline stops being used for rendering and gets deinitialized.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void deinitialize( RenderingPipelineMechanism& host ) {}
   
   /**
    * Called in order to perform all rendering-related things.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void update( RenderingPipelineMechanism& host ) {}

   // -------------------------------------------------------------------------
   // Connections management
   // -------------------------------------------------------------------------
   /**
    * Returns an array of nodes this node is connected to
    */
   inline const std::vector< RenderingPipelineNode* >& getNeighbors() const { return m_neighbors; }

   /**
    * Adds a neighbor to the list of neighbors.
    *
    * @param node
    */
   void addNeighbor( RenderingPipelineNode& node );

   /**
    * Removes a neighbor from the list of neighbors.
    *
    * @param node
    */
   void removeNeighbor( RenderingPipelineNode& node );
};

///////////////////////////////////////////////////////////////////////////////
