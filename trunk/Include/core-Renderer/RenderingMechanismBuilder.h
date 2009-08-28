#pragma once

/// @file   core-Renderer/RenderingMechanismBuilder.h
/// @brief  a functor for transforming rendering graphs into mechanisms

#include "core-Renderer\RenderingGraph.h"
#include "core-Renderer\CompositeRenderingMechanism.h"
#include <set>


///////////////////////////////////////////////////////////////////////////////

/**
 * This functor can be executed as a rendering graph operation. 
 * It will gather mechanisms from the respective nodes and build
 * a composite from them.
 */
class RenderingMechanismBuilder
{
private:
   CompositeRenderingMechanism* m_mech;
   std::set<RenderingMechanism*> m_sameMechFilter;

public:
   RenderingMechanismBuilder();
   ~RenderingMechanismBuilder();

   /**
    * Adds a new mechanism to the composite, providing the specified node
    * contains one.
    *
    * @param node    rendering graph node
    */
   void operator()(const RenderingNode& node);

   /**
    * The method returns the created composite. Once the method returns
    * the created mechanism, it creates a brand new one, allowing 
    * for the same instance to be used in another graph algorithm call.
    *
    * @return  created composite rendering mechanism
    */
   RenderingMechanism* get();
};

///////////////////////////////////////////////////////////////////////////////
