#pragma once

#include "core-Renderer\DeviceFilterElem.h"
#include "core-Renderer\RenderingDevice.h"


///////////////////////////////////////////////////////////////////////////////

class DevTypeFilter : public DeviceFilterElem
{
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
