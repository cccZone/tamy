#pragma once

#include <vector>
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class DeviceFilter;
struct RenderingDevice;

///////////////////////////////////////////////////////////////////////////////

class SimpleTamyConfigurator
{
public:
   RenderingDevice* selectRenderingDevice(IDirect3D9& d3d9,
                                          unsigned int width, 
                                          unsigned int height, 
                                          bool fullScreen);
};

///////////////////////////////////////////////////////////////////////////////
