#pragma once

#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * An implementation of this interface is responsible for providing
 * a feedback about a particular graphical device capabilities - whether they 
 * suit our needs or not
 */
class GraphicalCapsEvaluator
{
public:
   virtual ~GraphicalCapsEvaluator() {}

   virtual bool checkDeviceCaps(const D3DCAPS9& caps) = 0;
};

///////////////////////////////////////////////////////////////////////////////
