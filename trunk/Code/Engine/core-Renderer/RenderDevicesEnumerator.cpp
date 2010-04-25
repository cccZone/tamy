#include "core-Renderer\RenderDevicesEnumerator.h"
#include "core-Renderer\RenderingDevice.h"
#include <algorithm>
#include <stdexcept>
#include <string>


/////////////////////////////////////////////////////////////////////////////

RenderDevicesEnumerator::RenderDevicesEnumerator(IDirect3D9& d3d9)
      : m_d3d9(d3d9),
      m_devicesDB(NULL)
{
   m_d3d9.GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_windowedDisplayMode);
}

/////////////////////////////////////////////////////////////////////////////

void RenderDevicesEnumerator::getDevices(std::vector<RenderingDevice*>& devicesDB)
{
   m_devicesDB = &devicesDB;

   UINT adapterCount = m_d3d9.GetAdapterCount();
   for (UINT adapterNo = 0; adapterNo < adapterCount; adapterNo++)
   {
      enumerateAdapterFormats(adapterNo);
   }

   m_devicesDB = NULL;
}

/////////////////////////////////////////////////////////////////////////////

const UINT ValidAdapterFormatCount = 3;
const D3DFORMAT ValidAdapterFormats[3]  = {D3DFMT_X8R8G8B8, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5};

void RenderDevicesEnumerator::enumerateAdapterFormats(unsigned int adapterNo)
{
   for (UINT adapterFormatIdx = 0; adapterFormatIdx < ValidAdapterFormatCount; adapterFormatIdx++)
   {
      D3DFORMAT adapterFormat = ValidAdapterFormats[adapterFormatIdx];
      UINT adapterModeCount = m_d3d9.GetAdapterModeCount(adapterNo, adapterFormat);
      for (unsigned int adapterModeIdx = 0; adapterModeIdx < adapterModeCount; ++adapterModeIdx)
      {
         enumerateDisplayModes(adapterNo, adapterFormat, adapterModeIdx);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////

const UINT DeviceTypeCount = 3;
const D3DDEVTYPE DeviceTypes[DeviceTypeCount] = {D3DDEVTYPE_HAL, D3DDEVTYPE_SW, D3DDEVTYPE_REF};

void RenderDevicesEnumerator::enumerateDisplayModes(unsigned int adapterNo, 
                                                    D3DFORMAT adapterFmt, 
                                                    unsigned int dispModeIdx)
{
   D3DDISPLAYMODE displayMode;
   if (SUCCEEDED(m_d3d9.EnumAdapterModes(adapterNo, adapterFmt, dispModeIdx, &displayMode)))
   {
      for (UINT deviceTypeIdx = 0; deviceTypeIdx < DeviceTypeCount; deviceTypeIdx++)
      {
         D3DCAPS9 caps;
         if (FAILED(m_d3d9.GetDeviceCaps(adapterNo, DeviceTypes[deviceTypeIdx], &caps))) continue;

         enumerateDeviceOptions(adapterNo, adapterFmt, DeviceTypes[deviceTypeIdx], displayMode, caps);
      }
   }
}


/////////////////////////////////////////////////////////////////////////////
const UINT ValidBackBufferFormatsCount = 11;
const D3DFORMAT ValidBackBufferFormat[ValidBackBufferFormatsCount] = {
                                             D3DFMT_R8G8B8, D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, 
                                             D3DFMT_R5G6B5, D3DFMT_A1R5G5B5, D3DFMT_X1R5G5B5,
                                             D3DFMT_R3G3B2, D3DFMT_A8R3G3B2, D3DFMT_X4R4G4B4, 
                                             D3DFMT_A4R4G4B4, D3DFMT_A2B10G10R10};

void RenderDevicesEnumerator::enumerateDeviceOptions(unsigned int adapterNo,
                                                     D3DFORMAT adapterFormat,
                                                     D3DDEVTYPE devType,
                                                     D3DDISPLAYMODE displayMode,
                                                     D3DCAPS9& caps)
{
   for (UINT backBufferFormatIdx = 0; backBufferFormatIdx < ValidBackBufferFormatsCount; backBufferFormatIdx++)
   {
      D3DFORMAT backBufferFormat = ValidBackBufferFormat[backBufferFormatIdx];
      for (int windowed = 0; windowed <= 1; ++windowed)
      {
         bool isWindowed = (bool)windowed;

         if (isWindowed && (adapterFormat != m_windowedDisplayMode.Format)) continue;
         if (isWindowed && (adapterFormat != backBufferFormat)) continue;
         
         if (FAILED(m_d3d9.CheckDeviceType(adapterNo, devType, 
                                           adapterFormat, backBufferFormat, isWindowed))) continue;
       

         RenderingDevice* device = new RenderingDevice();
         device->windowed = isWindowed; 
         device->adapterFormat = adapterFormat;
         device->adapterOrdinal = adapterNo;
         device->caps = caps;
         device->deviceType = devType;
         device->displayMode = displayMode;
         device->backBufferFormat = backBufferFormat;
         
         // additional stuff
         enumerateDepthStencilFormats(*device);
         enumeratePresentationIntervals(*device);
         enumerateMultisamplingTypes(*device);

         std::vector<VERTEXPROCESSING_TYPE> types;
         enumerateVertexProcessingTypes(*device, types);

         if ((device->depthStencilFormats.size() > 0) &&
            (device->presentIntervals.size() > 0) &&
            (device->multisampleTypes.size() > 0))
         {
            unsigned int typesCount = types.size();
            for (unsigned int typeIdx = 0; typeIdx < typesCount; ++typeIdx)
            {
               RenderingDevice* newDevice = new RenderingDevice(*device); 
               newDevice->vertexProcessingType = types[typeIdx];
               m_devicesDB->push_back(newDevice);
            }
         }

         delete device;
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
const UINT DepthStencilFormatsCount = 6;
const D3DFORMAT DepthStencilFormats[DepthStencilFormatsCount]  = {
                                           D3DFMT_D32, D3DFMT_D24X4S4, D3DFMT_D24X8, 
                                           D3DFMT_D24S8, D3DFMT_D16, D3DFMT_D15S1};

void RenderDevicesEnumerator::enumerateDepthStencilFormats(RenderingDevice& device)
{

   for (UINT formatIdx = 0; formatIdx < DepthStencilFormatsCount; ++formatIdx)
   {
      if (SUCCEEDED(m_d3d9.CheckDeviceFormat(device.adapterOrdinal, device.deviceType, 
                                              device.adapterFormat, D3DUSAGE_DEPTHSTENCIL, 
                                              D3DRTYPE_SURFACE, DepthStencilFormats[formatIdx])))
      {
          if (SUCCEEDED(m_d3d9.CheckDepthStencilMatch(device.adapterOrdinal, device.deviceType, 
                                                       device.adapterFormat, device.backBufferFormat, 
                                                       DepthStencilFormats[formatIdx])))
          {
             device.depthStencilFormats.push_back(DepthStencilFormats[formatIdx]);
          }
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
const UINT PresentIntervalsCount = 6;
const ULONG PresentIntervals[PresentIntervalsCount]= {
                                  D3DPRESENT_INTERVAL_IMMEDIATE, D3DPRESENT_INTERVAL_DEFAULT,
                                  D3DPRESENT_INTERVAL_ONE, D3DPRESENT_INTERVAL_TWO,
                                  D3DPRESENT_INTERVAL_THREE, D3DPRESENT_INTERVAL_FOUR };

void RenderDevicesEnumerator::enumeratePresentationIntervals(RenderingDevice& device)
{
   for (UINT idx = 0; idx < PresentIntervalsCount; ++idx)
   {
      ULONG interval = PresentIntervals[idx];
      
      if ((device.windowed == true) &&
          ((interval == D3DPRESENT_INTERVAL_TWO) ||
           (interval == D3DPRESENT_INTERVAL_THREE) ||
           (interval == D3DPRESENT_INTERVAL_FOUR)))
      {
         continue;
      }

      if (interval == D3DPRESENT_INTERVAL_DEFAULT)
      {
         device.presentIntervals.push_back(PresentIntervals[idx]);
         continue;
      }

      if (device.caps.PresentationIntervals & PresentIntervals[idx])
      {
         device.presentIntervals.push_back(PresentIntervals[idx]);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////

const UINT MultiSampleTypesCount = 17;
const D3DMULTISAMPLE_TYPE MultiSampleTypes[MultiSampleTypesCount] = {
                                                  D3DMULTISAMPLE_NONE      , D3DMULTISAMPLE_NONMASKABLE,
                                                  D3DMULTISAMPLE_2_SAMPLES , D3DMULTISAMPLE_3_SAMPLES,
                                                  D3DMULTISAMPLE_4_SAMPLES , D3DMULTISAMPLE_5_SAMPLES,
                                                  D3DMULTISAMPLE_6_SAMPLES , D3DMULTISAMPLE_7_SAMPLES,
                                                  D3DMULTISAMPLE_8_SAMPLES , D3DMULTISAMPLE_9_SAMPLES,
                                                  D3DMULTISAMPLE_10_SAMPLES, D3DMULTISAMPLE_11_SAMPLES,
                                                  D3DMULTISAMPLE_12_SAMPLES, D3DMULTISAMPLE_13_SAMPLES,
                                                  D3DMULTISAMPLE_14_SAMPLES, D3DMULTISAMPLE_15_SAMPLES,
                                                  D3DMULTISAMPLE_16_SAMPLES};

void RenderDevicesEnumerator::enumerateMultisamplingTypes(RenderingDevice& device)
{
   for (UINT idx = 0; idx < MultiSampleTypesCount; idx++)
   {
      D3DMULTISAMPLE_TYPE multisampleType = MultiSampleTypes[idx];
      ULONG qualityLevel;

      if (FAILED(m_d3d9.CheckDeviceMultiSampleType(device.adapterOrdinal, device.deviceType, 
                                                    device.backBufferFormat, device.windowed, 
                                                    multisampleType, &qualityLevel))) continue;

      device.multisampleTypes.push_back(multisampleType);
      device.multisampleQualities.push_back(qualityLevel);
   }
}

/////////////////////////////////////////////////////////////////////////////

void RenderDevicesEnumerator::enumerateVertexProcessingTypes(RenderingDevice& device,
                                                             std::vector<VERTEXPROCESSING_TYPE>& types)
{
   if (device.caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
   {
      if (device.caps.DevCaps & D3DDEVCAPS_PUREDEVICE)
      {
         types.push_back(PURE_HARDWARE_VP);
      }
 
      types.push_back(HARDWARE_VP);
      types.push_back(MIXED_VP);
   }

   types.push_back(SOFTWARE_VP);
}

/////////////////////////////////////////////////////////////////////////////
