#include "RenderingProcessor.h"
#include "GraphicalNode.h"
#include "GraphicalEntity.h"
#include "Material.h"


///////////////////////////////////////////////////////////////////////////////

std::list<RenderingCommand> RenderingProcessor::translate(const BatchedNodes& nodesToRender)
{
   std::list<RenderingCommand> commands;
   Material* prevMat = NULL;

   for (BatchedNodes::const_iterator it = nodesToRender.begin();
        it != nodesToRender.end(); ++it)
   {
      GraphicalNode& graphicalNode = **it;
      Material& mat = graphicalNode.getMaterial();
      if ((prevMat == NULL) || (*prevMat != mat))
      {
         commands.push_back(setMaterial(mat));
         prevMat = &mat;
      }

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
