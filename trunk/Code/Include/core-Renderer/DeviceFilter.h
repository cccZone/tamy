#pragma once

#include <d3d9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

struct RenderingDevice;
class DeviceFilterElem;
class DevicesSorter;

///////////////////////////////////////////////////////////////////////////////

class DeviceFilter
{
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
