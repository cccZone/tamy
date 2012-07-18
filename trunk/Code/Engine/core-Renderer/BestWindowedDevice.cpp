#include "core-Renderer\BestWindowedDevice.h"
#include "core-Renderer\RenderingDevice.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

unsigned int BestWindowedDevice::runTests(const RenderingDevice& device) const
{
   if (device.windowed == false)
   {
      ASSERT_MSG( false, "We're trying to find the best windowed mode, and there are fullscreen modes on the list");
      return 0;
   }

   unsigned int score = 0;

   score += getDevTypeScore(device.deviceType);
   score += (device.backBufferFormat == device.displayMode.Format);

   return score;
}

///////////////////////////////////////////////////////////////////////////////

unsigned int BestWindowedDevice::getDevTypeScore(D3DDEVTYPE type) const
{
   if (type == D3DDEVTYPE_HAL) {return 1;}
   else {return 0;}
}

///////////////////////////////////////////////////////////////////////////////
