#pragma once

#include "core-Renderer\DeviceFilterElem.h"
#include "core-Renderer\RenderingDevice.h"


///////////////////////////////////////////////////////////////////////////////

class FullScreenModeFilter : public DeviceFilterElem
{
private:
   bool m_fullScreen;

public:
   FullScreenModeFilter(bool fullScreen) : m_fullScreen(fullScreen) {}

   bool validate(RenderingDevice& device) {return (device.windowed != m_fullScreen);}
};

///////////////////////////////////////////////////////////////////////////////
