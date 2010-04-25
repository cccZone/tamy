#pragma once

#include "core-Renderer\RenderingDevice.h"


///////////////////////////////////////////////////////////////////////////////

class DevicesSorter
{
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
