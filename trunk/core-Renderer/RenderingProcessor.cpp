#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\RenderingTechnique.h"


///////////////////////////////////////////////////////////////////////////////

void RenderingProcessor::translate(Array<AbstractGraphicalNode*>& nodesToRender)
{
   unsigned int startIdx = 0;
   unsigned int nodesCount = nodesToRender.size();
   if (nodesCount == 0) {return;}

   // we'll be processing the nodes in batches - find all the consecutive nodes
   // that share the technique, and when we spot the first one that uses a different technique,
   // let's render the ones we iterated over, and start counting anew...
   RenderingTechnique* prevTechnique = &(nodesToRender[0]->getTechnique());
   for (DWORD i = 1; i < nodesCount; ++i)
   {
      AbstractGraphicalNode& graphicalNode = *(nodesToRender[i]);
      RenderingTechnique& technique = graphicalNode.getTechnique();

      if (*prevTechnique != technique)
      {
         prevTechnique->render(nodesToRender, startIdx, i);
         startIdx = i;
         prevTechnique = &technique;
      }
   }

   // ...however that will leave us with the nodes at the end of the list
   // unprocessed - so process the remaining nodes now
   prevTechnique->render(nodesToRender, startIdx, nodesCount);
}

///////////////////////////////////////////////////////////////////////////////
