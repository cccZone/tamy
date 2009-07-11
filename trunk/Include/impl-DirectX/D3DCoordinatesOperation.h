#pragma once

#include "core-Renderer\CoordinatesOperation.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DCoordinatesOperation : public CoordinatesOperation
{
private:
   IDirect3DDevice9& m_d3Device;

public:
   D3DCoordinatesOperation(IDirect3DDevice9& d3Device, CoordsOpCode coordsOp);
   D3DCoordinatesOperation(const D3DCoordinatesOperation& rhs);

   CoordinatesOperation* clone();

   void setForRendering(unsigned char stageIdx);
};

///////////////////////////////////////////////////////////////////////////////
