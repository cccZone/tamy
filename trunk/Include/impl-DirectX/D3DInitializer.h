#pragma once

#include <windows.h>
#include <d3d9.h>
#include <vector>


class D3DRenderer;

enum VERTEXPROCESSING_TYPE
{
    SOFTWARE_VP         = 1,        // Software Vertex Processing
    MIXED_VP            = 2,        // Mixed Vertex Processing
    HARDWARE_VP         = 3,        // Hardware Vertex Processing
    PURE_HARDWARE_VP    = 4         // Pure Hardware Vertex Processing
};

//-----------------------------------------------------------------------------
// Name : D3DSettings (Support Class)
// Desc : Contains a set of options defining all possible display modes
//        for all the installed graphics adapters
//-----------------------------------------------------------------------------
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
};

//-----------------------------------------------------------------------------
// Name : DeviceOptions (Support Class)
// Desc : The structure stores the information about the capabilities
//        a device has
//-----------------------------------------------------------------------------
struct DeviceOptions
{
   ULONG                              adapterOrdinal;
   D3DDEVTYPE                         deviceType;
   D3DCAPS9                           caps;
   D3DFORMAT                          adapterFormat;
   D3DFORMAT                          backBufferFormat;
   std::vector<D3DFORMAT>             depthStencilFormats;
   std::vector<D3DMULTISAMPLE_TYPE>   multisampleTypes;
   std::vector<ULONG>                 multisampleQualities;
   std::vector<VERTEXPROCESSING_TYPE> vertexProcessingTypes;
   std::vector<ULONG>                 presentIntervals;
   bool                               windowed;
};

//-----------------------------------------------------------------------------
// Name : Device (Support Class)
// Desc : The structure stores the information about a device an adapter can use
//-----------------------------------------------------------------------------
struct Device
{
   ~Device();

   D3DDEVTYPE                   deviceType;
   D3DCAPS9                     caps;
   std::vector<DeviceOptions*> options;
};

//-----------------------------------------------------------------------------
// Name : Adapter (Support Class)
// Desc : The structure stores the information about an adapter present 
//        in the machine
//-----------------------------------------------------------------------------
struct Adapter
{
   ~Adapter();
   ULONG                       adapterOrdinal;
   std::vector<D3DDISPLAYMODE> displayModes;
   std::vector<Device*>       devices;
};

//-----------------------------------------------------------------------------
// Name : D3DInitializer (Class)
// Desc : Allows to create a graphics display suitable to our needs.
//-----------------------------------------------------------------------------
class D3DInitializer
{
private:
   IDirect3D9& m_d3d9;
   HWND m_focusWnd;

   std::vector<Adapter*> m_adapters;

public:
   D3DInitializer(IDirect3D9& d3d9, HWND focusWnd);
   ~D3DInitializer(void);

   D3DSettings findBestWindowedMode(bool bRequireHAL = false, bool bRequireREF = false);
   D3DSettings findBestFullscreenMode(D3DDISPLAYMODE& matchMode, bool bRequireHAL = false, bool bRequireREF = false);

   D3DRenderer* createDisplay(D3DSettings& settings, 
                              HWND hWnd = NULL,
                              ULONG Flags = 0);

private:
   void enumerateGraphicsEquipment();
   void enumerateDisplayModes(Adapter* adapter);
   void enumerateDevices(Adapter* adapter);
   void enumerateDeviceOptions(Adapter* adapter, Device* device);
   void enumerateDepthStencilFormats(DeviceOptions* options);
   void enumeratePresentationIntervals(DeviceOptions* options);
   void enumerateMultisamplingTypes(DeviceOptions* options);
   void enumerateVertexProcessingTypes(DeviceOptions* options);
   D3DFORMAT findOptimalTextureFormat(D3DSettings& settings);
};
