#pragma once

#include "core-Renderer\DevicesSorter.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

struct RenderingDevice;

///////////////////////////////////////////////////////////////////////////////

class BestFullScreenDevice : public DevicesSorter
{
protected:
   unsigned int runTests(const RenderingDevice& device) const;

private:
   unsigned int getDevTypeScore(D3DDEVTYPE type) const;
};

///////////////////////////////////////////////////////////////////////////////
