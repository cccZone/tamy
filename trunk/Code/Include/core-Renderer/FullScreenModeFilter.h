/// @file   core-Renderer/DevTypeFilter.h
/// @brief  full-screen mode filter
#pragma once

#include "core-Renderer\DeviceFilterElem.h"
#include "core-Renderer\RenderingDevice.h"


///////////////////////////////////////////////////////////////////////////////

class FullScreenModeFilter : public DeviceFilterElem
{
   DECLARE_ALLOCATOR( FullScreenModeFilter, AM_DEFAULT );

private:
   bool m_fullScreen;

public:
   FullScreenModeFilter(bool fullScreen) : m_fullScreen(fullScreen) {}

   bool validate(RenderingDevice& device) {return (device.windowed != m_fullScreen);}
};

///////////////////////////////////////////////////////////////////////////////
