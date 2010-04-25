#pragma once

#include <windows.h>
#include <d3d9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;
enum VERTEXPROCESSING_TYPE;
struct RenderingDevice;

///////////////////////////////////////////////////////////////////////////////

/**
 * Contains a set of options defining all possible display modes
 * for all the installed graphics adapters
 */
struct DX9Settings
{
   ULONG                   adapterOrdinal;
   D3DDISPLAYMODE          displayMode;
   D3DDEVTYPE              deviceType;
   D3DFORMAT               backBufferFormat;
   D3DFORMAT               depthStencilFormat;
   D3DMULTISAMPLE_TYPE     multisampleType;
   ULONG                   multisampleQuality;
   VERTEXPROCESSING_TYPE   vertexProcessingType;
   ULONG                   presentInterval;
   bool                    windowed;
   D3DCAPS9                caps;

   DX9Settings(RenderingDevice& device);
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Allows to create a graphics display suitable to our needs.
 */
class DX9Initializer
{
private:
   IDirect3D9& m_d3d9;

public:
   DX9Initializer(IDirect3D9& d3d9);

   DX9Renderer* createDisplay(DX9Settings& settings, 
                              HWND hWnd,
                              ULONG Flags = 0);

private:

   D3DFORMAT findOptimalTextureFormat(DX9Settings& settings);
};

///////////////////////////////////////////////////////////////////////////////
