#ifndef _LIGHTS_SORTING_STRATEGY_H
#error "This file can only be included from LightSortingStorage.h"
#else

#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

template <typename Sorter>
LightsSortingStorage<Sorter>::LightsSortingStorage(LightsStorage* storage,
                                                   Sorter* sorter)
: m_storage(storage)
, m_sorter(sorter)
{
   if (m_sorter == NULL)
   {
      m_sorter = new Sorter();
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename Sorter>
LightsSortingStorage<Sorter>::~LightsSortingStorage()
{
   delete m_sorter;
   m_sorter = NULL;

   delete m_storage;
   m_storage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template <typename Sorter>
void LightsSortingStorage<Sorter>::query(const D3DXVECTOR3& point, 
                                         Array<Light*>& outLights)
{
   m_storage->query(point, outLights);

   m_sorter->setInfluencedPos(point);
   std::sort((Light**)outLights, 
             (Light**)outLights + outLights.size(), 
             *m_sorter);
}

///////////////////////////////////////////////////////////////////////////////

#endif // _LIGHTS_SORTING_STRATEGY_H
