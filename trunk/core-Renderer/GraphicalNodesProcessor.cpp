#include "core-Renderer\GraphicalNodesProcessor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\RenderingTechnique.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

void GraphicalNodesProcessor::operator()(const Array<AbstractGraphicalNode*>& input, 
                                         const Frustum& frustum,
                                         const D3DXVECTOR3& cameraGlobalPos,
                                         Array<AbstractGraphicalNode*>& output)
{
   // segregate nodes
   output.clear();
   unsigned int nodesCount = input.size();
   unsigned int regularNodesCount = 0;
   for (unsigned int i = 0; i < nodesCount; ++i)
   {
      if (input[i]->getTechnique().isTransparent() == false)
      {
         output.push_back(input[i]);
         regularNodesCount++;
      }
   }

   for (unsigned int i = 0; i < nodesCount; ++i)
   {
      if (input[i]->getTechnique().isTransparent() == true)
      {
         output.push_back(input[i]);
      }
   }

   // sort the nodes
   std::sort((AbstractGraphicalNode**)output, 
      (AbstractGraphicalNode**)output + regularNodesCount,
      m_materialsComparator);


   m_distanceComparator.setReferencePoint(cameraGlobalPos);
   std::sort((AbstractGraphicalNode**)output + regularNodesCount, 
      (AbstractGraphicalNode**)output + output.size(),
      m_distanceComparator);
}

///////////////////////////////////////////////////////////////////////////////
