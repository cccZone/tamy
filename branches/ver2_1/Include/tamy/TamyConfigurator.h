#pragma once

#include <vector>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

struct RenderingDevice;

///////////////////////////////////////////////////////////////////////////////

/**
 * User should provide an implemetation of this interface
 * to Tamy's initialization method in order to set it up as he pleases
 */
class TamyConfigurator
{
public:
   virtual ~TamyConfigurator() {}

   virtual RenderingDevice& selectRenderingDevice(const std::vector<RenderingDevice*>& devicesDB) = 0;

   virtual HICON getApplicationIcon() = 0;
};

///////////////////////////////////////////////////////////////////////////////
