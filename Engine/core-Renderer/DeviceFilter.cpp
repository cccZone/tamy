#include "core-Renderer\DeviceFilter.h"
#include "core-Renderer\DeviceFilterElem.h"
#include "core-Renderer\RenderingDevice.h"
#include "core-Renderer\DevicesSorter.h"
#include <windows.h>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

class NullDevicesSorter : public DevicesSorter
{
protected:
   unsigned int runTests(const RenderingDevice& device) const
   {
      return device.adapterOrdinal;
   }
};

///////////////////////////////////////////////////////////////////////////////

DeviceFilter::DeviceFilter()
      : m_sorter(new NullDevicesSorter())
{
}

///////////////////////////////////////////////////////////////////////////////

DeviceFilter::~DeviceFilter()
{
   unsigned int count = m_filters.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_filters[i];
   }

   delete m_sorter;
   m_sorter = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DeviceFilter::add(DeviceFilterElem* filter)
{
   m_filters.push_back(filter);
}

///////////////////////////////////////////////////////////////////////////////

void DeviceFilter::set(DevicesSorter* sorter)
{
   delete m_sorter;
   m_sorter = sorter;
}

///////////////////////////////////////////////////////////////////////////////

void DeviceFilter::operator()(const std::vector<RenderingDevice*>& in, 
                              std::vector<RenderingDevice*>& out)
{
   unsigned int devicesCount = in.size();
   unsigned int filtersCount = m_filters.size();
   bool valid;
   for (unsigned int i = 0; i < devicesCount; ++i)
   {
      RenderingDevice* device = in[i];

      valid = true;
      for (unsigned int filterIdx = 0; filterIdx < filtersCount; ++filterIdx)
      {
         if (m_filters[filterIdx]->validate(*device) == false) 
         {
            valid = false;
            break;
         }
      }
      if (valid == true) 
      {
         out.push_back(device);
      }
   }

   std::sort(out.begin(), out.end(), *m_sorter);
}

///////////////////////////////////////////////////////////////////////////////
