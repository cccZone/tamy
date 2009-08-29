#include "tamy\BestWindowedDevice.h"
#include "core-Renderer\RenderingDevice.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

unsigned int BestWindowedDevice::runTests(const RenderingDevice& device) const
{
   if (device.windowed == false)
   {
      throw std::runtime_error("We're trying to find the best windowed mode, \
                               and there are fullscreen modes on the list");
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
