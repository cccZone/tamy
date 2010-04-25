#pragma once

#include "core-Renderer\DeviceFilterElem.h"
#include "core-Renderer\RenderingDevice.h"


///////////////////////////////////////////////////////////////////////////////

class ResolutionFilter : public DeviceFilterElem
{
private:
   unsigned int m_width;
   unsigned int m_height;

public:
   ResolutionFilter(unsigned int width, unsigned int height)
      : m_width(width), m_height(height)
   {}

   bool validate(RenderingDevice& device)
   {
      return (device.displayMode.Width == m_width) && (device.displayMode.Height == m_height);
   }
};

///////////////////////////////////////////////////////////////////////////////
