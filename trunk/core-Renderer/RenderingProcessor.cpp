#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\Material.h"


///////////////////////////////////////////////////////////////////////////////

DWORD RenderingProcessor::translate(AbstractGraphicalNodeP* nodesToRender, const DWORD& nodesArraySize,
                                    RenderingCommand* renderingCommands, const DWORD& commandsArraySize)
{
   Material* prevMat = NULL;
   DWORD commandIdx = 0;

   for (DWORD i = 0; (i < nodesArraySize) && (commandIdx < commandsArraySize); ++i)
   {
      if (nodesToRender[i] == NULL) {continue;}

      AbstractGraphicalNode& graphicalNode = *(nodesToRender[i]);
      Material& mat = graphicalNode.getMaterial();

      if ((prevMat == NULL) || (*prevMat != mat))
      {
         renderingCommands[commandIdx++] = setMaterial(mat);
         prevMat = &mat;
      }
      renderingCommands[commandIdx++] = renderEntity(graphicalNode);
   }

   return commandIdx;

}

///////////////////////////////////////////////////////////////////////////////

RenderingCommand RenderingProcessor::setMaterial(Material& material)
{
   return RenderingCommand::from_method<Material, &Material::setForRendering> (&material);
}

///////////////////////////////////////////////////////////////////////////////

RenderingCommand RenderingProcessor::renderEntity(AbstractGraphicalNode& graphicalNode)
{
   return RenderingCommand::from_method<AbstractGraphicalNode, &AbstractGraphicalNode::render> (&graphicalNode);
}

///////////////////////////////////////////////////////////////////////////////
