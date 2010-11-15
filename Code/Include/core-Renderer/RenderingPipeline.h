/// @file   core-Renderer/RenderingPipeline.h
/// @brief  rendering pipeline resource

#pragma once

#include "core/Resource.h"
#include "core/Graph.h"


///////////////////////////////////////////////////////////////////////////////

// TEMP: this is a temporary structure for now ...
struct TestNode
{
   DECLARE_RTTI_STRUCT
};

struct TestEdge
{
   DECLARE_RTTI_STRUCT
};

///////////////////////////////////////////////////////////////////////////////

class RenderingPipeline : public Resource
{
   DECLARE_RESOURCE( RenderingPipeline );



public:
   /**
    * Constructor.
    *
    * @param fileName   name of the resource file
    */
   RenderingPipeline( const std::string& fileName = "" );

   /**
    * Adds a new node to the pipeline.
    *
    * @param node       node to add
    */
   void addNode( TestNode* node );

   /**
    * Removes a node from the pipeline
    *
    * @param node       node to remove
    */
   void removeNode( TestNode& node );
};

///////////////////////////////////////////////////////////////////////////////
