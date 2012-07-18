#include "core-Renderer\BestFullScreenDevice.h"
#include "core-Renderer\RenderingDevice.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////


unsigned int BestFullScreenDevice::runTests(const RenderingDevice& device) const
{
   if (device.windowed == true)
   {
      ASSERT_MSG( false, "We're trying to find the best fullscreen mode, and there are windowed modes on the list");
      return 0;
   }

   unsigned int score = 0;

   score += getDevTypeScore(device.deviceType);
   score += (device.backBufferFormat == device.displayMode.Format);
   score += (device.adapterFormat == device.displayMode.Format);

   return score;
}

///////////////////////////////////////////////////////////////////////////////

unsigned int BestFullScreenDevice::getDevTypeScore(D3DDEVTYPE type) const
{
   if (type == D3DDEVTYPE_HAL) {return 1;}
   else {return 0;}
}

///////////////////////////////////////////////////////////////////////////////
