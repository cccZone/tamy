#ifndef VISUAL_SCENE_MANAGER_H
#error "Include from VisualSceneManager.h only."
#else

#include "core-Renderer\LinearNodesStorage.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\AbstractGraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

template<typename QueryVolume>
void VisualSceneManager::query(const QueryVolume& volume, Array<AbstractGraphicalNode*>& output) const
{
   m_staticNodesContainer->query(volume, output);
   m_dynamicNodesContainer->query(volume, output);
}

///////////////////////////////////////////////////////////////////////////////

template<typename QueryVolume>
void VisualSceneManager::detailedQuery(const QueryVolume& volume, Array<AbstractGraphicalNode*>& output) const
{
   Array<AbstractGraphicalNode*> possiblyVisibleNodes;
   query(volume, possiblyVisibleNodes);

   // perform a narrow phase query
   unsigned int nodesCount = possiblyVisibleNodes.size();
   unsigned int trisCount;
   Array<Triangle*> geometry;

   AbstractGraphicalNode* node = NULL;
   bool visible = false;
   D3DXMATRIX volumeTransformMtx;
   for (unsigned int i = 0; i < nodesCount; ++i)
   {
      node = possiblyVisibleNodes[i];

      visible = false;
      geometry.clear();
      node->getEntity().getGeometry(geometry);

      trisCount = geometry.size();
      D3DXMatrixInverse(&volumeTransformMtx, NULL, &(node->getGlobalMtx()));
      for (unsigned int j = 0; j < trisCount; ++j)
      {
         QueryVolume transformedVolume = volume * volumeTransformMtx;
         Triangle* tri = geometry[j];
         if (testCollision(*tri, transformedVolume) == true) 
         {
            visible = true;
            break;
         }
      }

      if (visible)
      {
         output.push_back(node);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif
