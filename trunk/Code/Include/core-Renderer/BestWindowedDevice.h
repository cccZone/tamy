/// @file   core-Renderer/BestFullScreenDevice.h
/// @brief  best rendering device for in-window rendering
#pragma once

#include "core-Renderer\DevicesSorter.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

struct RenderingDevice;

///////////////////////////////////////////////////////////////////////////////

class BestWindowedDevice : public DevicesSorter
{
   DECLARE_ALLOCATOR( BestWindowedDevice, AM_DEFAULT );

protected:
   unsigned int runTests(const RenderingDevice& device) const;

private:
   unsigned int getDevTypeScore(D3DDEVTYPE type) const;
};

///////////////////////////////////////////////////////////////////////////////
