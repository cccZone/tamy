#pragma once

/// @file   core-Renderer/RenderingGraph.h
/// @brief  graph describing a rendering pipeline configuration

#include "core\Graph.h"


///////////////////////////////////////////////////////////////////////////////

struct RenderingNode;
struct RenderingEdge;
class RenderingMechanism;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering graph definition. Graph defines the flow that will process a scene
 * into an image that can be displayed on the screen.
 */
typedef Graph<RenderingNode, RenderingEdge> RenderingGraph;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering graph node definition. Each node contains 
 * a dedicated rendering mechanism.
 */
struct RenderingNode
{
   RenderingMechanism* mech;

   RenderingNode()
      : mech(NULL)
   {}

   RenderingNode(RenderingMechanism* _mech)
      : mech(_mech)
   {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering graph edge definition.
 */
struct RenderingEdge
{
   RenderingGraph::Index endNodeIdx;

   RenderingEdge(RenderingGraph::Index _endNodeIdx)
      : endNodeIdx(_endNodeIdx) 
   {}

   operator RenderingGraph::Index() const {return endNodeIdx;}
};

///////////////////////////////////////////////////////////////////////////////
