#include "impl-DirectX\D3DAlphaOperationImplementation.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

D3DAlphaOperationImplementation::D3DAlphaOperationImplementation(IDirect3DDevice9& d3Device)
      : m_d3Device(d3Device)
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::setLightReflectingProperties(unsigned char stage, unsigned char argIdx)
{
   m_d3Device.SetTextureStageState(stage, translateArgNum(argIdx), D3DTA_DIFFUSE);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::setTexture(unsigned char stage, unsigned char argIdx)
{
   m_d3Device.SetTextureStageState(stage, translateArgNum(argIdx), D3DTA_TEXTURE);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::setPreviousStageResults(unsigned char stage, unsigned char argIdx)
{
   m_d3Device.SetTextureStageState(stage, translateArgNum(argIdx), D3DTA_CURRENT);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::disableArgument(unsigned char stage, unsigned char argIdx)
{
   m_d3Device.SetTextureStageState(stage, translateArgNum(argIdx), D3DTA_SELECTMASK);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::disableStage(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::selectArg1(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::selectArg2(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::multiply(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::multiply2X(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::multiply4X(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_MODULATE4X);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::add(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_ADD);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::addSigned(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_ADDSIGNED);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::addSigned2X(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_ADDSIGNED2X);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::addSmooth(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_ADDSMOOTH);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::subtract(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_SUBTRACT);
}

///////////////////////////////////////////////////////////////////////////////

void D3DAlphaOperationImplementation::multiplyAdd(unsigned char stage)
{
   m_d3Device.SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_MULTIPLYADD);
}

///////////////////////////////////////////////////////////////////////////////
