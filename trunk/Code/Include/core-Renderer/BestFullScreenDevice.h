/// @file   core-Renderer/BestFullScreenDevice.h
/// @brief  best rendering device for full-screen rendering
#pragma once

#include "core-Renderer\DevicesSorter.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

struct RenderingDevice;

///////////////////////////////////////////////////////////////////////////////

class BestFullScreenDevice : public DevicesSorter
{
   DECLARE_ALLOCATOR( BestFullScreenDevice, AM_DEFAULT );

protected:
   unsigned int runTests(const RenderingDevice& device) const;

private:
   unsigned int getDevTypeScore(D3DDEVTYPE type) const;
};

///////////////////////////////////////////////////////////////////////////////
