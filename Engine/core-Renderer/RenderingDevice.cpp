#include "core-Renderer\RenderingDevice.h"


///////////////////////////////////////////////////////////////////////////////

std::string RenderingDevice::getDesc() const
{
   char tmpStr[1024];

   sprintf_s(tmpStr, 1024, 
   "Device no. %d \n"                           \
   "\tType %s\n"                                \
   "\tFullscreen %s\n"                          \
   "\tMode\n"                                   \
   "\t\tdisplay - [%d, %d, %d, %s]\n"           \
   "\t\tadapter fmt - %s\n"                     \
   "\t\tback buffer fmt - %s\n"                 \
   "\t\tdepth stencil fmt - %s\n"               \
   "\t\tvertex processing model - %s\n"         \
   "\tCapabilities\n"                           \
   "\t\tpixel shader %d.%d\n"                   \
   "\t\tvertex shader %d.%d",
   adapterOrdinal,
   descDevType(deviceType).c_str(),
   (windowed ? "NO" : "YES"),
   displayMode.Width, displayMode.Height, displayMode.RefreshRate, descFormat(displayMode.Format).c_str(),
   descFormat(adapterFormat).c_str(),
   descFormat(backBufferFormat).c_str(),
   descFormat(depthStencilFormats[0]).c_str(),
   descVtxProcessType(vertexProcessingType).c_str(),
   D3DSHADER_VERSION_MAJOR(caps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(caps.PixelShaderVersion),
   D3DSHADER_VERSION_MAJOR(caps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(caps.VertexShaderVersion));

   return tmpStr;
}

///////////////////////////////////////////////////////////////////////////////

std::string RenderingDevice::descFormat(const D3DFORMAT& fmt) const
{
   switch(fmt)
   {
   case D3DFMT_R8G8B8: return "D3DFMT_R8G8B8"; break;
   case D3DFMT_A8R8G8B8: return "D3DFMT_A8R8G8B8"; break;
   case D3DFMT_X8R8G8B8: return "D3DFMT_X8R8G8B8"; break;
   case D3DFMT_R5G6B5: return "D3DFMT_R5G6B5"; break;
   case D3DFMT_X1R5G5B5: return "D3DFMT_X1R5G5B5"; break;
   case D3DFMT_A1R5G5B5: return "D3DFMT_A1R5G5B5"; break;
   case D3DFMT_A4R4G4B4: return "D3DFMT_A4R4G4B4"; break;
   case D3DFMT_R3G3B2: return "D3DFMT_R3G3B2"; break;
   case D3DFMT_A8: return "D3DFMT_A8"; break;
   case D3DFMT_A8R3G3B2: return "D3DFMT_A8R3G3B2"; break;
   case D3DFMT_X4R4G4B4: return "D3DFMT_X4R4G4B4"; break;
   case D3DFMT_A2B10G10R10: return "D3DFMT_A2B10G10R10"; break;
   case D3DFMT_A8B8G8R8: return "D3DFMT_A8B8G8R8"; break;
   case D3DFMT_X8B8G8R8: return "D3DFMT_X8B8G8R8"; break;
   case D3DFMT_G16R16: return "D3DFMT_G16R16"; break;
   case D3DFMT_A2R10G10B10: return "D3DFMT_A2R10G10B10"; break;
   case D3DFMT_A16B16G16R16: return "D3DFMT_A16B16G16R16"; break;
   case D3DFMT_A8P8: return "D3DFMT_A8P8"; break;
   case D3DFMT_P8: return "D3DFMT_P8"; break;
   case D3DFMT_L8: return "D3DFMT_L8"; break;
   case D3DFMT_A8L8: return "D3DFMT_A8L8"; break;
   case D3DFMT_A4L4: return "D3DFMT_A4L4"; break;
   case D3DFMT_V8U8: return "D3DFMT_V8U8"; break;
   case D3DFMT_L6V5U5: return "D3DFMT_L6V5U5"; break;
   case D3DFMT_X8L8V8U8: return "D3DFMT_X8L8V8U8"; break;
   case D3DFMT_Q8W8V8U8: return "D3DFMT_Q8W8V8U8"; break;
   case D3DFMT_V16U16: return "D3DFMT_V16U16"; break;
   case D3DFMT_A2W10V10U10: return "D3DFMT_A2W10V10U10"; break;
   case D3DFMT_UYVY: return "D3DFMT_UYVY"; break;
   case D3DFMT_R8G8_B8G8: return "D3DFMT_R8G8_B8G8"; break;
   case D3DFMT_YUY2: return "D3DFMT_YUY2"; break;
   case D3DFMT_G8R8_G8B8: return "D3DFMT_G8R8_G8B8"; break;
   case D3DFMT_DXT1: return "D3DFMT_DXT1"; break;
   case D3DFMT_DXT2: return "D3DFMT_DXT2"; break;
   case D3DFMT_DXT3: return "D3DFMT_DXT3"; break;
   case D3DFMT_DXT4: return "D3DFMT_DXT4"; break;
   case D3DFMT_DXT5: return "D3DFMT_DXT5"; break;
   case D3DFMT_D16_LOCKABLE: return "D3DFMT_D16_LOCKABLE"; break;
   case D3DFMT_D32: return "D3DFMT_D32"; break;
   case D3DFMT_D15S1: return "D3DFMT_D15S1"; break;
   case D3DFMT_D24S8: return "D3DFMT_D24S8"; break;
   case D3DFMT_D24X8: return "D3DFMT_D24X8"; break;
   case D3DFMT_D24X4S4: return "D3DFMT_D24X4S4"; break;
   case D3DFMT_D16: return "D3DFMT_D16"; break;
   case D3DFMT_D32F_LOCKABLE: return "D3DFMT_D32F_LOCKABLE"; break;
   case D3DFMT_D24FS8: return "D3DFMT_D24FS8"; break;
   case D3DFMT_D32_LOCKABLE: return "D3DFMT_D32_LOCKABLE"; break;
   case D3DFMT_S8_LOCKABLE: return "D3DFMT_S8_LOCKABLE"; break;
   case D3DFMT_L16: return "D3DFMT_L16"; break;
   case D3DFMT_VERTEXDATA: return "D3DFMT_VERTEXDATA"; break;
   case D3DFMT_INDEX16: return "D3DFMT_INDEX16"; break;
   case D3DFMT_INDEX32: return "D3DFMT_INDEX32"; break;
   case D3DFMT_Q16W16V16U16: return "D3DFMT_Q16W16V16U16"; break;
   case D3DFMT_MULTI2_ARGB8: return "D3DFMT_MULTI2_ARGB8"; break;
   case D3DFMT_R16F: return "D3DFMT_R16F"; break;
   case D3DFMT_G16R16F: return "D3DFMT_G16R16F"; break;
   case D3DFMT_A16B16G16R16F: return "D3DFMT_A16B16G16R16F"; break;
   case D3DFMT_R32F: return "D3DFMT_R32F"; break;
   case D3DFMT_G32R32F: return "D3DFMT_G32R32F"; break;
   case D3DFMT_A32B32G32R32F: return "D3DFMT_A32B32G32R32F"; break;
   case D3DFMT_CxV8U8: return "D3DFMT_CxV8U8"; break;
   case D3DFMT_A1: return "D3DFMT_A1"; break;
   case D3DFMT_BINARYBUFFER: return "D3DFMT_BINARYBUFFER"; break;
   case D3DFMT_UNKNOWN: 
   default: return "D3DFMT_UNKNOWN"; break;
   }

}

///////////////////////////////////////////////////////////////////////////////

std::string RenderingDevice::descDevType(const D3DDEVTYPE& type) const
{
   switch(type)
   {
   case D3DDEVTYPE_HAL: return "D3DDEVTYPE_HAL"; break;
   case D3DDEVTYPE_REF: return "D3DDEVTYPE_REF"; break;
   case D3DDEVTYPE_SW: return "D3DDEVTYPE_SW"; break;
   case D3DDEVTYPE_NULLREF: return "D3DDEVTYPE_NULLREF"; break;
   default: return "Unknown"; break;
   }

}

///////////////////////////////////////////////////////////////////////////////

std::string RenderingDevice::descVtxProcessType(const VERTEXPROCESSING_TYPE& type) const
{
   switch(type)
   {
   case SOFTWARE_VP: return "SOFTWARE_VP"; break;
   case MIXED_VP: return "MIXED_VP"; break;
   case HARDWARE_VP: return "HARDWARE_VP"; break;
   case PURE_HARDWARE_VP: return "PURE_HARDWARE_VP"; break;
   default: return "Unknown"; break;
   }
}

///////////////////////////////////////////////////////////////////////////////
