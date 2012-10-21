/// @file   core-Renderer/DeviceFilter.h
/// @brief  rendering device filter
#pragma once

#include <vector>
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

struct RenderingDevice;
class DeviceFilterElem;
class DevicesSorter;

///////////////////////////////////////////////////////////////////////////////

class DeviceFilter
{
   DECLARE_ALLOCATOR( DeviceFilter, AM_DEFAULT );

private:
   std::vector<DeviceFilterElem*> m_filters;
   DevicesSorter* m_sorter;

public:
   DeviceFilter();
   virtual ~DeviceFilter();

   void add(DeviceFilterElem* filter);
   void set(DevicesSorter* sorter);

   void operator()(const std::vector<RenderingDevice*>& in, std::vector<RenderingDevice*>& out);
};

///////////////////////////////////////////////////////////////////////////////
