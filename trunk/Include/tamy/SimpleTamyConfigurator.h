#pragma once

#include "tamy\TamyConfigurator.h"


///////////////////////////////////////////////////////////////////////////////

class DeviceFilter;

///////////////////////////////////////////////////////////////////////////////

class SimpleTamyConfigurator : public TamyConfigurator
{
private:
   unsigned int m_width;
   unsigned int m_height;
   bool m_fullScreen;

public:
   SimpleTamyConfigurator(unsigned int width, unsigned int height, bool fullScreen);

   RenderingDevice& selectRenderingDevice(const std::vector<RenderingDevice*>& devicesDB);
};

///////////////////////////////////////////////////////////////////////////////
