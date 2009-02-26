#pragma once

#include "NodeVisitor.h"
#include "TNodesVisitor.h"
#include "Light.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class is used during the rendering process to extract
 * the lights in the scene
 */
class LightsAggregator : public NodeVisitor, public TNodesVisitor<Light>
{
private:
   std::list<Light*> m_lights;

public:
   void visit(Light& light) {m_lights.push_back(&light);}

   const std::list<Light*>& operator()() const {return m_lights;}
};

///////////////////////////////////////////////////////////////////////////////
