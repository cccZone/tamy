#pragma once

#include <windows.h>
#include <d3d9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class D3DRenderer;
enum VERTEXPROCESSING_TYPE;
struct RenderingDevice;

///////////////////////////////////////////////////////////////////////////////

/**
 * Contains a set of options defining all possible display modes
 * for all the installed graphics adapters
 */
struct D3DSettings
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

   D3DSettings(RenderingDevice& device);
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Allows to create a graphics display suitable to our needs.
 */
class D3DInitializer
{
private:
   IDirect3D9& m_d3d9;

public:
   D3DInitializer(IDirect3D9& d3d9);

   D3DRenderer* createDisplay(D3DSettings& settings, 
                              HWND hWnd,
                              ULONG Flags = 0);

private:

   D3DFORMAT findOptimalTextureFormat(D3DSettings& settings);
};

///////////////////////////////////////////////////////////////////////////////
