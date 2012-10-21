/// @file   core-Renderer/SimpleTamyConfigurator.h
/// @brief  simple rendering device configurator
#pragma once

#include <vector>
#include <d3d9.h>
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

class DeviceFilter;
struct RenderingDevice;

///////////////////////////////////////////////////////////////////////////////

class SimpleTamyConfigurator
{
   DECLARE_ALLOCATOR( SimpleTamyConfigurator, AM_DEFAULT );

public:
   RenderingDevice* selectRenderingDevice( IDirect3D9& d3d9, unsigned int width, unsigned int height, bool fullScreen );
};

///////////////////////////////////////////////////////////////////////////////
