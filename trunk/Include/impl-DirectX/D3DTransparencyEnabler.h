#pragma once

#include "core-Renderer\TransparencyEnabler.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DTransparencyEnabler : public TransparencyEnabler
{
private:
   IDirect3DDevice9& m_d3Device;

public:
   D3DTransparencyEnabler(IDirect3DDevice9& d3Device);

   void setTransparency(bool enable);
};

///////////////////////////////////////////////////////////////////////////////
