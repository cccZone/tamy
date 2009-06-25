#pragma once

#include "core-Renderer\Material.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DMaterial : public Material
{
private:
   IDirect3DDevice9& m_d3Device;

public:
   D3DMaterial(IDirect3DDevice9& d3Device, 
               const std::string& name,
               LightReflectingProperties* lrp,  
               MaterialOperationImplementation& alphaMatOp,
               MaterialOperationImplementation& colorMatOp,
               unsigned int index);

protected:
   void enableTransparency(bool enable);
};

///////////////////////////////////////////////////////////////////////////////
