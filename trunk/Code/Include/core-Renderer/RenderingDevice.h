#pragma once

#include <windows.h>
#include <d3d9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

struct DX9Settings;

///////////////////////////////////////////////////////////////////////////////

enum VERTEXPROCESSING_TYPE
{
    SOFTWARE_VP         = 1,        // Software Vertex Processing
    MIXED_VP            = 2,        // Mixed Vertex Processing
    HARDWARE_VP         = 3,        // Hardware Vertex Processing
    PURE_HARDWARE_VP    = 4         // Pure Hardware Vertex Processing
};

///////////////////////////////////////////////////////////////////////////////

/**
 * The structure stores the information about the capabilities
 * a device has
 */
struct RenderingDevice
{
   D3DDISPLAYMODE                     displayMode;
   ULONG                              adapterOrdinal;
   D3DDEVTYPE                         deviceType;
   D3DCAPS9                           caps;
   D3DFORMAT                          adapterFormat;
   D3DFORMAT                          backBufferFormat;
   std::vector<D3DFORMAT>             depthStencilFormats;
   std::vector<D3DMULTISAMPLE_TYPE>   multisampleTypes;
   std::vector<ULONG>                 multisampleQualities;
   VERTEXPROCESSING_TYPE              vertexProcessingType;
   std::vector<ULONG>                 presentIntervals;
   bool                               windowed;

   std::string getDesc() const;

private:
   std::string descFormat(const D3DFORMAT& fmt) const;
   std::string descDevType(const D3DDEVTYPE& type) const;
   std::string descVtxProcessType(const VERTEXPROCESSING_TYPE& type) const;
};

///////////////////////////////////////////////////////////////////////////////
