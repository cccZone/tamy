#include "impl-DirectX\D3DColorOperationImplementation.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

D3DColorOperationImplementation::D3DColorOperationImplementation(IDirect3DDevice9& d3Device)
      : m_d3Device(d3Device)
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::setLightReflectingProperties(unsigned char stage, unsigned char argIdx)
{
   m_d3Device.SetTextureStageState(stage, translateArgNum(argIdx), D3DTA_DIFFUSE);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::setTexture(unsigned char stage, unsigned char argIdx)
{
   m_d3Device.SetTextureStageState(stage, translateArgNum(argIdx), D3DTA_TEXTURE);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::setPreviousStageResults(unsigned char stage, unsigned char argIdx)
{
   m_d3Device.SetTextureStageState(stage, translateArgNum(argIdx), D3DTA_CURRENT);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::disableArgument(unsigned char stage, unsigned char argIdx)
{
   m_d3Device.SetTextureStageState(stage, translateArgNum(argIdx), D3DTA_SELECTMASK);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::disableStage(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_DISABLE);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::selectArg1(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::selectArg2(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::multiply(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_MODULATE);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::multiply2X(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::multiply4X(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_MODULATE4X);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::add(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_ADD);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::addSigned(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::addSigned2X(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_ADDSIGNED2X);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::addSmooth(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_ADDSMOOTH);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::subtract(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
}

///////////////////////////////////////////////////////////////////////////////

void D3DColorOperationImplementation::multiplyAdd(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);
}

///////////////////////////////////////////////////////////////////////////////
