#pragma once

#include <d3d9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

struct RenderingDevice;
enum VERTEXPROCESSING_TYPE;

///////////////////////////////////////////////////////////////////////////////

class RenderDevicesEnumerator
{
private:
   IDirect3D9& m_d3d9;
   D3DDISPLAYMODE m_windowedDisplayMode;
   std::vector<RenderingDevice*>* m_devicesDB;

public:
   RenderDevicesEnumerator(IDirect3D9& d3d9);
   void getDevices(std::vector<RenderingDevice*>& devicesDB);

private:
   void enumerateAdapterFormats(unsigned int adapterNo);
   void enumerateDisplayModes(unsigned int adapterNo, 
                              D3DFORMAT adapterFmt, 
                              unsigned int dispModeIdx);
   void enumerateDeviceOptions(unsigned int adapterNo,
                               D3DFORMAT adapterFormat,
                               D3DDEVTYPE devType,
                               D3DDISPLAYMODE displayMode,
                               D3DCAPS9& caps);
   void enumerateDepthStencilFormats(RenderingDevice& device);
   void enumeratePresentationIntervals(RenderingDevice& device);
   void enumerateMultisamplingTypes(RenderingDevice& device);
   void enumerateVertexProcessingTypes(RenderingDevice& device, 
                                       std::vector<VERTEXPROCESSING_TYPE>& types);
};

///////////////////////////////////////////////////////////////////////////////
