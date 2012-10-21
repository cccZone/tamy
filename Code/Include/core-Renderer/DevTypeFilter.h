/// @file   core-Renderer/DevTypeFilter.h
/// @brief  device type filter
#pragma once

#include "core-Renderer\DeviceFilterElem.h"
#include "core-Renderer\RenderingDevice.h"


///////////////////////////////////////////////////////////////////////////////

class DevTypeFilter : public DeviceFilterElem
{
   DECLARE_ALLOCATOR( DevTypeFilter, AM_DEFAULT );

private:
   D3DDEVTYPE m_devType;

public:
   DevTypeFilter(D3DDEVTYPE devType) 
      : m_devType(devType)
   {}

   bool validate(RenderingDevice& device)
   {
      return (m_devType == device.deviceType);
   }

};

///////////////////////////////////////////////////////////////////////////////
