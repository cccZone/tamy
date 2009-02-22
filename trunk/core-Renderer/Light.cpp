#include "Light.h"
#include "GraphicalNodesAggregator.h"


///////////////////////////////////////////////////////////////////////////////

Light::Light(const std::string& name)
      : Node(name)
{
}

///////////////////////////////////////////////////////////////////////////////

void Light::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(GraphicalNodesAggregator<Light>);
}

///////////////////////////////////////////////////////////////////////////////
