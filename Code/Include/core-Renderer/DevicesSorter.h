/// @file   core-Renderer/DeviceSorter.h
/// @brief  abstract class for implementing rendering devices selection mechanisms
#pragma once

#include "core-Renderer\RenderingDevice.h"
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

class DevicesSorter
{
   DECLARE_ALLOCATOR( DevicesSorter, AM_DEFAULT );

public:
   virtual ~DevicesSorter() {}

   bool operator()(RenderingDevice* lhs, RenderingDevice* rhs) const
   {
      unsigned int lhsScore = 0;
      unsigned int rhsScore = 0;

      lhsScore = runTests(*lhs);
      rhsScore = runTests(*rhs);

      return lhsScore > rhsScore;
   }

protected:
   virtual unsigned int runTests(const RenderingDevice& device) const {return device.adapterOrdinal;}
};

///////////////////////////////////////////////////////////////////////////////
