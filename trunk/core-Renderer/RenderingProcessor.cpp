#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include "core-Renderer\MaterialImpl.h"


///////////////////////////////////////////////////////////////////////////////

void RenderingProcessor::translate(Array<AbstractGraphicalNode*>& nodesToRender,
                                   MaterialImpl& impl,
                                   RenderingTargetsPolicy& policy)
{
   unsigned int nodesCount = nodesToRender.size();
   if (nodesCount == 0) {return;}
   if (policy.getDefinedPassesCount() == 0) {return;}

   policy.setTargets(0);

   // we'll be processing the nodes in batches - find all the consecutive nodes
   // that share the material, and when we spot the first one that uses a different material,
   // let's render the ones we iterated over, and start counting anew...
   Material* prevMaterial = NULL;
   for (DWORD i = 0; i < nodesCount; ++i)
   {
      AbstractGraphicalNode& graphicalNode = *(nodesToRender[i]);
      Material& material = graphicalNode.getMaterial();

      if ((!prevMaterial) || (*prevMaterial != material))
      {
         material.setForRendering(impl);
         prevMaterial = &material;
      }
      graphicalNode.render();
   }
}

///////////////////////////////////////////////////////////////////////////////
