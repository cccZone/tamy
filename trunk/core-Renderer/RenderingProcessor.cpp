#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\Material.h"


///////////////////////////////////////////////////////////////////////////////

void RenderingProcessor::translate(AbstractGraphicalNodeP* nodesToRender, const DWORD& nodesArraySize)
{
   Material* prevMat = NULL;
   DWORD commandIdx = 0;

   for (DWORD i = 0; i < nodesArraySize; ++i)
   {
      if (nodesToRender[i] == NULL) {continue;}

      AbstractGraphicalNode& graphicalNode = *(nodesToRender[i]);
      Material& mat = graphicalNode.getMaterial();

      if ((prevMat == NULL) || (*prevMat != mat))
      {
         mat.setForRendering();
         prevMat = &mat;
      }
      graphicalNode.render();
   }
}

///////////////////////////////////////////////////////////////////////////////
