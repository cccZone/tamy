#include "core-Renderer\SimpleTamyConfigurator.h"
#include "core-Renderer\DeviceFilter.h"
#include "core-Renderer\RenderDevicesEnumerator.h"

// filters
#include "core-Renderer\FullScreenModeFilter.h"
#include "core-Renderer\ResolutionFilter.h"
#include "core-Renderer\DevTypeFilter.h"

// sorters
#include "core-Renderer\BestFullScreenDevice.h"
#include "core-Renderer\BestWindowedDevice.h"

#include "core/Assert.h"



///////////////////////////////////////////////////////////////////////////////

RenderingDevice* SimpleTamyConfigurator::selectRenderingDevice(IDirect3D9& d3d9, unsigned int width, unsigned int height, bool fullScreen)
{
   std::vector<RenderingDevice*> allDevicesDB;
   RenderDevicesEnumerator enumerator(d3d9);
   enumerator.getDevices(allDevicesDB);

   // filter out the devices our engine intrinsically can't use due to
   // the way certain operations are implemented
   DeviceFilter filter;
   filter.add(new FullScreenModeFilter(fullScreen));
   filter.add(new DevTypeFilter(D3DDEVTYPE_HAL));
   filter.add(new ResolutionFilter(width, height));

   if (fullScreen)
   {
      filter.set(new BestFullScreenDevice());
   }
   else
   {
      filter.set(new BestWindowedDevice());
   }

   std::vector<RenderingDevice*> applicableDevices;
   filter(allDevicesDB, applicableDevices);

   if (applicableDevices.size() == 0)
   {
      ASSERT_MSG( false, "No applicable graphics device found" );
      return NULL;
   }

   RenderingDevice* ourDevice = new RenderingDevice(*(applicableDevices[0]));

   // cleanup and return the device descriptor
   unsigned int allDevicesCount = allDevicesDB.size();
   for (unsigned int i = 0; i < allDevicesCount; ++i)
   {
      delete allDevicesDB[i];
   }

   return ourDevice;
}

///////////////////////////////////////////////////////////////////////////////
