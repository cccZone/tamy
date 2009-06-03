#include "core-Renderer\OctreeSpatialContainer.h"


///////////////////////////////////////////////////////////////////////////////

OctreeSpatialContainer::OctreeSpatialContainer(unsigned int maxElemsPerSector, 
                                               float worldSize)
      : m_nodesContainer(new Octree<AbstractGraphicalNodeP, AGNVolExtractor>(maxElemsPerSector, 
                                                                             worldSize))
{}

///////////////////////////////////////////////////////////////////////////////

OctreeSpatialContainer::~OctreeSpatialContainer()
{
   delete m_nodesContainer;
   m_nodesContainer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void OctreeSpatialContainer::insert(const AbstractGraphicalNodeP& elem)
{
   m_nodesContainer->insert(elem);
}

///////////////////////////////////////////////////////////////////////////////

void OctreeSpatialContainer::remove(const AbstractGraphicalNodeP& elem)
{
   m_nodesContainer->remove(elem);
}

///////////////////////////////////////////////////////////////////////////////

void OctreeSpatialContainer::query(const Frustum& boundingVol, 
                                   Array<AbstractGraphicalNodeP>& output) const
{
   m_nodesContainer->query(boundingVol, output);
}

///////////////////////////////////////////////////////////////////////////////
