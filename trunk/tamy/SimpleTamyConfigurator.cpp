#include "tamy\SimpleTamyConfigurator.h"
#include "core-Renderer\DeviceFilter.h"

// filters
#include "tamy\FullScreenModeFilter.h"
#include "tamy\ResolutionFilter.h"
#include "tamy\DevTypeFilter.h"

// sorters
#include "tamy\BestFullScreenDevice.h"
#include "tamy\BestWindowedDevice.h"

#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

SimpleTamyConfigurator::SimpleTamyConfigurator(unsigned int width, 
                                               unsigned int height, 
                                               bool fullScreen)
      : m_width(width),
      m_height(height),
      m_fullScreen(fullScreen)
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingDevice& 
SimpleTamyConfigurator::selectRenderingDevice(const std::vector<RenderingDevice*>& devicesDB)
{
   DeviceFilter filter;

   filter.add(new FullScreenModeFilter(m_fullScreen));
   filter.add(new DevTypeFilter(D3DDEVTYPE_HAL));
   filter.add(new ResolutionFilter(m_width, m_height));

   if (m_fullScreen)
   {
      filter.set(new BestFullScreenDevice());
   }
   else
   {
      filter.set(new BestWindowedDevice());
   }

   std::vector<RenderingDevice*> applicableDevices;
   filter(devicesDB, applicableDevices);

   if (applicableDevices.size() == 0)
   {
      throw std::runtime_error("No applicable graphics device found");
   }

   return *(applicableDevices[0]);
}

///////////////////////////////////////////////////////////////////////////////
