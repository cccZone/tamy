#include "impl-DirectX\D3DCoordinatesOperation.h"


///////////////////////////////////////////////////////////////////////////////

D3DCoordinatesOperation::D3DCoordinatesOperation(IDirect3DDevice9& d3Device, 
                                                 CoordsOpCode coordsOp)
      : CoordinatesOperation(coordsOp),
      m_d3Device(d3Device)
{
}
///////////////////////////////////////////////////////////////////////////////

D3DCoordinatesOperation::D3DCoordinatesOperation(const D3DCoordinatesOperation& rhs)
      : CoordinatesOperation(rhs),
      m_d3Device(rhs.m_d3Device)
{
}

///////////////////////////////////////////////////////////////////////////////

CoordinatesOperation* D3DCoordinatesOperation::clone()
{
   return new D3DCoordinatesOperation(*this);
}

///////////////////////////////////////////////////////////////////////////////

void D3DCoordinatesOperation::setForRendering(unsigned char stageIdx)
{
   DWORD d3dCode;
   switch(getCode())
   {
   case CC_WRAP:        d3dCode = D3DTADDRESS_WRAP;         break;
   case CC_MIRROR:      d3dCode = D3DTADDRESS_MIRROR;       break;
   case CC_CLAMP:       d3dCode = D3DTADDRESS_CLAMP;        break;
   case CC_BORDER:      d3dCode = D3DTADDRESS_BORDER;       break;
   case CC_MIRRORONCE:  d3dCode = D3DTADDRESS_MIRRORONCE;   break;
   default:             d3dCode = D3DTADDRESS_WRAP;         break;
   }

   m_d3Device.SetSamplerState(stageIdx, D3DSAMP_ADDRESSU, d3dCode);
   m_d3Device.SetSamplerState(stageIdx, D3DSAMP_ADDRESSV, d3dCode);
}

///////////////////////////////////////////////////////////////////////////////
