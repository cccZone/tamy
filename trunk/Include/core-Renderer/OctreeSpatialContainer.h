#pragma once

#include "core-Renderer\SpatialContainer.h"
#include "core\Octree.h"
#include "core-Renderer\AGNVolExtractor.h"


///////////////////////////////////////////////////////////////////////////////

class OctreeSpatialContainer : public SpatialContainer
{
private:
   Octree<AbstractGraphicalNodeP, AGNVolExtractor>* m_nodesContainer;

public:
   OctreeSpatialContainer(unsigned int maxElemsPerSector = 64, float worldSize = 1000);
   ~OctreeSpatialContainer();

   void insert(const AbstractGraphicalNodeP& elem);

   void remove(const AbstractGraphicalNodeP& elem);

   void query(const Frustum& boundingVol, 
              Array<AbstractGraphicalNodeP>& output) const;
};

///////////////////////////////////////////////////////////////////////////////
