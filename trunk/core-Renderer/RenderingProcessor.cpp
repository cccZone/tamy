#include "RenderingProcessor.h"
#include "GraphicalNode.h"
#include "GraphicalEntity.h"
#include "Material.h"


///////////////////////////////////////////////////////////////////////////////

std::list<RenderingCommand> RenderingProcessor::translate(const std::list<GraphicalNode*>& nodesToRender)
{
   std::list<RenderingCommand> commands;

   for (std::list<GraphicalNode*>::const_iterator it = nodesToRender.begin();
                                                  it != nodesToRender.end(); 
                                                  ++it)
   {
      GraphicalNode& graphicalNode = **it;
   
      commands.push_back(setMaterial(graphicalNode.getMaterial()));
      commands.push_back(renderEntity(graphicalNode));
   }

   return commands;
}

///////////////////////////////////////////////////////////////////////////////

RenderingCommand RenderingProcessor::setMaterial(Material& material)
{
   return RenderingCommand::from_method<Material, &Material::setForRendering> (&material);
}

///////////////////////////////////////////////////////////////////////////////

RenderingCommand RenderingProcessor::renderEntity(GraphicalNode& graphicalNode)
{
   return RenderingCommand::from_method<GraphicalNode, &GraphicalNode::render> (&graphicalNode);
}

///////////////////////////////////////////////////////////////////////////////
