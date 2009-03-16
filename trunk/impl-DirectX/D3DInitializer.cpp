#include "impl-DirectX\D3DInitializer.h"
#include "impl-DirectX\D3DRenderer.h"
#include <algorithm>
#include <stdexcept>
#include <string>


/////////////////////////////////////////////////////////////////////////////

Device::~Device()
{
   for (UINT optionNo = 0; optionNo < options.size(); optionNo++)
   {
      delete options[optionNo];
   }
   options.clear();
}

/////////////////////////////////////////////////////////////////////////////

Adapter::~Adapter()
{
   for (UINT deviceNo = 0; deviceNo < devices.size(); deviceNo++)
   {
      delete devices[deviceNo];
   }
   devices.clear();
}

/////////////////////////////////////////////////////////////////////////////

D3DInitializer::D3DInitializer(IDirect3D9& d3d9, HWND focusWnd)
      : m_d3d9(d3d9),
      m_focusWnd(focusWnd)
{
   enumerateGraphicsEquipment();
}

/////////////////////////////////////////////////////////////////////////////

D3DInitializer::~D3DInitializer(void)
{
   for (UINT adapterNo = 0; adapterNo < m_adapters.size(); adapterNo++)
   {
      delete m_adapters[adapterNo];
   }
   m_adapters.clear();
}

/////////////////////////////////////////////////////////////////////////////

D3DSettings D3DInitializer::findBestWindowedMode(bool bRequireHAL, bool bRequireREF)
{
   DeviceOptions   *bestOptions = NULL;

   D3DDISPLAYMODE currentDisplayMode;
   m_d3d9.GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &currentDisplayMode);

   for (UINT adapterIdx = 0; adapterIdx < m_adapters.size(); adapterIdx++)
   {
      Adapter* adapter = m_adapters[adapterIdx];

      for (UINT deviceIdx = 0; deviceIdx < adapter->devices.size(); deviceIdx++)
      {
         Device* device = adapter->devices[deviceIdx];

         if (bRequireHAL && (device->deviceType != D3DDEVTYPE_HAL)) continue;
         if (bRequireREF && (device->deviceType != D3DDEVTYPE_REF)) continue;

         for (UINT optIdx = 0; optIdx < device->options.size(); optIdx++)
         {
            DeviceOptions* options = device->options[optIdx];

            if (options->windowed == false) continue;
            if (options->adapterFormat != currentDisplayMode.Format) continue;

            bool backBufferMatch = (options->backBufferFormat == currentDisplayMode.Format);

            if ((options->deviceType == D3DDEVTYPE_HAL) && backBufferMatch)
            {
               bestOptions = options;
               goto FoundBestWindowedMode;
            }
            else if ((bestOptions == NULL) ||
                     ((bestOptions->deviceType != D3DDEVTYPE_HAL) && (options->deviceType == D3DDEVTYPE_HAL)))
            {
               bestOptions = options;
            }
         }
      }
   }

FoundBestWindowedMode:

   if (bestOptions == NULL)
   {
      throw std::logic_error(
         std::string("No windowed mode is available on current equipment"));
   }

   D3DSettings settings;
   settings.adapterOrdinal = bestOptions->adapterOrdinal;
   settings.backBufferFormat = bestOptions->backBufferFormat;
   settings.depthStencilFormat = bestOptions->depthStencilFormats[0];
   settings.deviceType = bestOptions->deviceType;
   settings.displayMode = currentDisplayMode;
   settings.multisampleType = bestOptions->multisampleTypes[0];
   settings.multisampleQuality = bestOptions->multisampleQualities[0];
   settings.presentInterval = bestOptions->presentIntervals[0];
   settings.vertexProcessingType = bestOptions->vertexProcessingTypes[0];
   settings.windowed = true;
   settings.caps = bestOptions->caps;

   return settings;
}

/////////////////////////////////////////////////////////////////////////////

D3DSettings D3DInitializer::findBestFullscreenMode(D3DDISPLAYMODE& matchMode, bool bRequireHAL, bool bRequireREF)
{
   DeviceOptions* bestOptions = NULL;
   D3DDISPLAYMODE bestDisplayMode;
   ZeroMemory(&bestDisplayMode, sizeof(D3DDISPLAYMODE));

   for (UINT adapterIdx = 0; adapterIdx < m_adapters.size(); adapterIdx++)
   {
      Adapter* adapter = m_adapters[adapterIdx];

      D3DDISPLAYMODE currentDisplayMode;
      m_d3d9.GetAdapterDisplayMode(adapter->adapterOrdinal, &currentDisplayMode);

      if (matchMode.Width  != 0) currentDisplayMode.Width  = matchMode.Width;
      if (matchMode.Height != 0) currentDisplayMode.Height = matchMode.Height;
      if (matchMode.Format != D3DFMT_UNKNOWN) currentDisplayMode.Format = matchMode.Format;
      if (matchMode.RefreshRate != 0) currentDisplayMode.RefreshRate = matchMode.RefreshRate;

      for (UINT deviceIdx = 0; deviceIdx < adapter->devices.size(); deviceIdx++)
      {
         Device* device = adapter->devices[deviceIdx];

         if (bRequireHAL && (device->deviceType != D3DDEVTYPE_HAL)) continue;
         if (bRequireREF && (device->deviceType != D3DDEVTYPE_REF)) continue;

         for (UINT optIdx = 0; optIdx < device->options.size(); optIdx++)
         {
            DeviceOptions* options = device->options[optIdx];

            if (options->windowed == true) continue;

            bool modeMatch = (options->adapterFormat == currentDisplayMode.Format);
            bool backBufferMatch = (options->backBufferFormat == options->adapterFormat);

            if (bestOptions == NULL ||
               (bestOptions->deviceType != D3DDEVTYPE_HAL && device->deviceType == D3DDEVTYPE_HAL ) ||
               (options->deviceType == D3DDEVTYPE_HAL && bestOptions->adapterFormat != currentDisplayMode.Format && modeMatch) ||
               (options->deviceType == D3DDEVTYPE_HAL && modeMatch && backBufferMatch))
            {
               bestDisplayMode = currentDisplayMode;
               bestOptions = options;

               if (options->deviceType == D3DDEVTYPE_HAL && modeMatch && backBufferMatch)
               {
                  goto FoundBestFullscreenMode;
               }

            }  
         }
      }
   }

FoundBestFullscreenMode:

   if (bestOptions == NULL)
   {
      throw std::logic_error(
         std::string("No windowed mode is available on current equipment"));
   }

   D3DSettings settings;
   settings.adapterOrdinal = bestOptions->adapterOrdinal;
   settings.backBufferFormat = bestOptions->backBufferFormat;
   settings.depthStencilFormat = bestOptions->depthStencilFormats[0];
   settings.deviceType = bestOptions->deviceType;
   settings.displayMode = bestDisplayMode;
   settings.multisampleType = bestOptions->multisampleTypes[0];
   settings.multisampleQuality = bestOptions->multisampleQualities[0];
   settings.presentInterval = bestOptions->presentIntervals[0];
   settings.vertexProcessingType = bestOptions->vertexProcessingTypes[0];
   settings.windowed = false;
   settings.caps = bestOptions->caps;

   return settings;
}

/////////////////////////////////////////////////////////////////////////////

D3DRenderer* D3DInitializer::createDisplay(D3DSettings& settings,  
                                            HWND hWnd, 
                                            ULONG Flags)
{
   D3DPRESENT_PARAMETERS presentParams;
   ZeroMemory(&presentParams, sizeof(D3DPRESENT_PARAMETERS));

   presentParams.EnableAutoDepthStencil = true;
   presentParams.AutoDepthStencilFormat = settings.depthStencilFormat;
   presentParams.hDeviceWindow = hWnd;
   presentParams.Windowed = settings.windowed;
   presentParams.PresentationInterval = settings.presentInterval;
   presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
   presentParams.BackBufferCount = 0;
   presentParams.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
   presentParams.BackBufferFormat = settings.backBufferFormat;

   UINT width, height;
   if (settings.windowed)
   {
      RECT rc;
      ::GetClientRect(hWnd, &rc);
      width = rc.right - rc.left;
      height = rc.bottom - rc.top;
   }
   else
   {
      width = settings.displayMode.Width;
      height = settings.displayMode.Height;
   }

   presentParams.BackBufferWidth = width;
   presentParams.BackBufferHeight = height;

   ULONG creationFlags = 0;
   bool hardwareTLOn;
   switch(settings.vertexProcessingType)
   {
   case SOFTWARE_VP:
      {
         creationFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
         hardwareTLOn = false;
         break;
      }

   case MIXED_VP:
      {
         creationFlags |= D3DCREATE_MIXED_VERTEXPROCESSING;
         hardwareTLOn = false;
         break;
      }

   case HARDWARE_VP:
      {
         creationFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
         hardwareTLOn = true;
         break;
      }

   case PURE_HARDWARE_VP:
      {
         creationFlags |= D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING;
         hardwareTLOn = true;
         break;
      }
   }

   D3DFORMAT optimalTextureFormat = findOptimalTextureFormat(settings);

   IDirect3DDevice9* d3Device = NULL;
   HRESULT deviceCreationResult = m_d3d9.CreateDevice(settings.adapterOrdinal, 
                                                       settings.deviceType, 
                                                       m_focusWnd, 
                                                       creationFlags, 
                                                       &presentParams, 
                                                       &d3Device);
   if (FAILED(deviceCreationResult))
   {
      return NULL;
   }
   else
   {
      return new D3DRenderer(d3Device, 
                             presentParams, 
                             settings.caps,
                             width,
                             height,
                             hardwareTLOn,
                             optimalTextureFormat);
   }
}

/////////////////////////////////////////////////////////////////////////////
void D3DInitializer::enumerateGraphicsEquipment()
{
   UINT adapterCount = m_d3d9.GetAdapterCount();
   for (UINT adapterNo = 0; adapterNo < adapterCount; adapterNo++)
   {
      Adapter* adapter = new Adapter();
      adapter->adapterOrdinal = adapterNo;

      enumerateDisplayModes(adapter);
      enumerateDevices(adapter);

      if ((adapter->devices.size() == 0) ||
         (adapter->displayModes.size() == 0))
      {
         delete adapter;
      }
      else
      {
         m_adapters.push_back(adapter);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
const UINT ValidAdapterFormatCount = 3;
const D3DFORMAT ValidAdapterFormats[3]  = {D3DFMT_X8R8G8B8, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5};

void D3DInitializer::enumerateDisplayModes(Adapter* adapter)
{
   for (UINT adapterFormatIdx = 0; adapterFormatIdx < ValidAdapterFormatCount; adapterFormatIdx++)
   {
      D3DFORMAT adapterFormat = ValidAdapterFormats[adapterFormatIdx];
      UINT adapterModeCount = m_d3d9.GetAdapterModeCount(adapter->adapterOrdinal, adapterFormat);
      for (UINT adapterMode = 0; adapterMode < adapterModeCount; adapterMode++)
      {
         D3DDISPLAYMODE displayMode;
         if (SUCCEEDED(m_d3d9.EnumAdapterModes(adapter->adapterOrdinal, adapterFormat, adapterMode, &displayMode)))
         {
            adapter->displayModes.push_back(displayMode);
         }
      }
   }
}

/////////////////////////////////////////////////////////////////////////////

const UINT DeviceTypeCount = 3;
const D3DDEVTYPE DeviceTypes[3] = {D3DDEVTYPE_HAL, D3DDEVTYPE_SW, D3DDEVTYPE_REF};

void D3DInitializer::enumerateDevices(Adapter* adapter)
{
   for (UINT deviceTypeIdx = 0; deviceTypeIdx < DeviceTypeCount; deviceTypeIdx++)
   {
      D3DCAPS9 deviceCaps;
      if (FAILED(m_d3d9.GetDeviceCaps(adapter->adapterOrdinal, DeviceTypes[deviceTypeIdx], &deviceCaps))) continue;

      Device* device = new Device();
      device->deviceType = DeviceTypes[deviceTypeIdx];
      device->caps = deviceCaps;
      enumerateDeviceOptions(adapter, device);

      if (device->options.size() == 0)
      {
         delete device;
      }
      else
      {
         adapter->devices.push_back(device);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
const UINT ValidBackBufferFormatsCount = 11;
const D3DFORMAT ValidBackBufferFormat[11] = {D3DFMT_R8G8B8, D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, 
                                             D3DFMT_R5G6B5, D3DFMT_A1R5G5B5, D3DFMT_X1R5G5B5,
                                             D3DFMT_R3G3B2, D3DFMT_A8R3G3B2, D3DFMT_X4R4G4B4, 
                                             D3DFMT_A4R4G4B4, D3DFMT_A2B10G10R10};

void D3DInitializer::enumerateDeviceOptions(Adapter* adapter, Device* device)
{
   std::vector<D3DFORMAT> adapterFormats;

   // prepare a list of all the backbuffer formats the selected adapter
   // supports across all it's modes
   for (UINT adapterModeIdx = 0; adapterModeIdx < adapter->displayModes.size(); adapterModeIdx++)
   {
      if (std::find(adapterFormats.begin(), 
                    adapterFormats.end(), 
                    adapter->displayModes[adapterModeIdx].Format) == adapterFormats.end())
      {
         adapterFormats.push_back(adapter->displayModes[adapterModeIdx].Format);
      }
   }

   // check the options for the adapter and all the backbuffer formats it supports
   for (UINT adapterFormatIdx = 0; adapterFormatIdx < adapterFormats.size(); adapterFormatIdx++)
   {
      D3DFORMAT adapterFormat = adapterFormats[adapterFormatIdx];

      for (UINT backBufferFormatIdx = 0; backBufferFormatIdx < ValidBackBufferFormatsCount; backBufferFormatIdx++)
      {
         D3DFORMAT backBufferFormat = ValidBackBufferFormat[backBufferFormatIdx];

         for (int windowed = 0; windowed <= 1; windowed++)
         {
            bool isWindowed = (windowed == 1) ? true : false;
            if (FAILED(m_d3d9.CheckDeviceType(adapter->adapterOrdinal, device->deviceType, 
                                               adapterFormat, backBufferFormat, isWindowed))) continue;

            DeviceOptions* options = new DeviceOptions();

            options->windowed = isWindowed;        
            options->adapterOrdinal = adapter->adapterOrdinal;
            options->deviceType = device->deviceType;
            options->caps = device->caps;
            options->adapterFormat = adapterFormat;
            options->backBufferFormat = backBufferFormat;

            enumerateDepthStencilFormats(options);
            enumeratePresentationIntervals(options);
            enumerateMultisamplingTypes(options);
            enumerateVertexProcessingTypes(options);

            if ((options->depthStencilFormats.size() == 0) ||
               (options->presentIntervals.size() == 0) ||
               (options->multisampleTypes.size() == 0) ||
               (options->vertexProcessingTypes.size() == 0))
            {
               delete options;
            }
            else
            {
               device->options.push_back(options);
            }
         }
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
const UINT DepthStencilFormatsCount = 6;
const D3DFORMAT DepthStencilFormats[6]  = {D3DFMT_D32, D3DFMT_D24X4S4, D3DFMT_D24X8, 
                                           D3DFMT_D24S8, D3DFMT_D16, D3DFMT_D15S1};

void D3DInitializer::enumerateDepthStencilFormats(DeviceOptions* options)
{

   for (UINT formatIdx = 0; formatIdx < DepthStencilFormatsCount; formatIdx++)
   {
      if (SUCCEEDED(m_d3d9.CheckDeviceFormat(options->adapterOrdinal, options->deviceType, 
                                              options->adapterFormat, D3DUSAGE_DEPTHSTENCIL, 
                                              D3DRTYPE_SURFACE, DepthStencilFormats[formatIdx])))
      {
          if (SUCCEEDED(m_d3d9.CheckDepthStencilMatch(options->adapterOrdinal, options->deviceType, 
                                                       options->adapterFormat, options->backBufferFormat, 
                                                       DepthStencilFormats[formatIdx])))
          {
             options->depthStencilFormats.push_back(DepthStencilFormats[formatIdx]);
          }
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
const UINT PresentIntervalsCount = 6;
const ULONG PresentIntervals[6]= {D3DPRESENT_INTERVAL_IMMEDIATE, D3DPRESENT_INTERVAL_DEFAULT,
                                  D3DPRESENT_INTERVAL_ONE, D3DPRESENT_INTERVAL_TWO,
                                  D3DPRESENT_INTERVAL_THREE, D3DPRESENT_INTERVAL_FOUR };

void D3DInitializer::enumeratePresentationIntervals(DeviceOptions* options)
{
   for (UINT idx = 0; idx < PresentIntervalsCount; idx++)
   {
      ULONG interval = PresentIntervals[idx];
      
      if ((options->windowed == true) &&
          ((interval == D3DPRESENT_INTERVAL_TWO) ||
           (interval == D3DPRESENT_INTERVAL_THREE) ||
           (interval == D3DPRESENT_INTERVAL_FOUR)))
      {
         continue;
      }

      if (interval == D3DPRESENT_INTERVAL_DEFAULT)
      {
         options->presentIntervals.push_back(PresentIntervals[idx]);
         continue;
      }

      if (options->caps.PresentationIntervals & PresentIntervals[idx])
      {
         options->presentIntervals.push_back(PresentIntervals[idx]);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
const UINT MultiSampleTypesCount = 17;
const D3DMULTISAMPLE_TYPE MultiSampleTypes[17] = {D3DMULTISAMPLE_NONE      , D3DMULTISAMPLE_NONMASKABLE,
                                                  D3DMULTISAMPLE_2_SAMPLES , D3DMULTISAMPLE_3_SAMPLES,
                                                  D3DMULTISAMPLE_4_SAMPLES , D3DMULTISAMPLE_5_SAMPLES,
                                                  D3DMULTISAMPLE_6_SAMPLES , D3DMULTISAMPLE_7_SAMPLES,
                                                  D3DMULTISAMPLE_8_SAMPLES , D3DMULTISAMPLE_9_SAMPLES,
                                                  D3DMULTISAMPLE_10_SAMPLES, D3DMULTISAMPLE_11_SAMPLES,
                                                  D3DMULTISAMPLE_12_SAMPLES, D3DMULTISAMPLE_13_SAMPLES,
                                                  D3DMULTISAMPLE_14_SAMPLES, D3DMULTISAMPLE_15_SAMPLES,
                                                  D3DMULTISAMPLE_16_SAMPLES};

void D3DInitializer::enumerateMultisamplingTypes(DeviceOptions* options)
{
   for (UINT idx = 0; idx < MultiSampleTypesCount; idx++)
   {
      D3DMULTISAMPLE_TYPE multisampleType = MultiSampleTypes[idx];
      ULONG qualityLevel;

      if (FAILED(m_d3d9.CheckDeviceMultiSampleType(options->adapterOrdinal, options->deviceType, 
                                                    options->backBufferFormat, options->windowed, 
                                                    multisampleType, &qualityLevel))) continue;

      options->multisampleTypes.push_back(multisampleType);
      options->multisampleQualities.push_back(qualityLevel);
   }
}

/////////////////////////////////////////////////////////////////////////////

void D3DInitializer::enumerateVertexProcessingTypes(DeviceOptions* options)
{
   if (options->caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
   {
      if (options->caps.DevCaps & D3DDEVCAPS_PUREDEVICE)
      {
         options->vertexProcessingTypes.push_back(PURE_HARDWARE_VP);
      }
 
      options->vertexProcessingTypes.push_back(HARDWARE_VP);
      options->vertexProcessingTypes.push_back(MIXED_VP);
   }

   options->vertexProcessingTypes.push_back(SOFTWARE_VP);
}

/////////////////////////////////////////////////////////////////////////////
const UINT TextureFormatsCount = 4;
const D3DFORMAT TextureFormats[4] = {D3DFMT_DXT1, D3DFMT_X8R8G8B8, D3DFMT_R5G6B5,
                                     D3DFMT_X1R5G5B5};

D3DFORMAT D3DInitializer::findOptimalTextureFormat(D3DSettings& settings)
{
   for (UINT i = 0; i < TextureFormatsCount; ++i)
   {
      if (SUCCEEDED(m_d3d9.CheckDeviceFormat(settings.adapterOrdinal, 
                                             settings.deviceType, 
                                             settings.displayMode.Format,
                                             0, D3DRTYPE_TEXTURE, 
                                             TextureFormats[i])))
      {
         return TextureFormats[i];
      }
   }

   return D3DFMT_UNKNOWN;
}

/////////////////////////////////////////////////////////////////////////////
