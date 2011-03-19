#pragma once

#include "core-Renderer\Renderer.h"
#include "core-Renderer\ShaderTexture.h"
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "core\Subject.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * DX9Renderer will emit these signals to its observers
 * when it needs them to release or recreate all the
 * resources that were created in the D3DPOOL_DEFAULT pool.
 */
enum DX9GraphResourceOp
{
   GRO_RELEASE_RES,
   GRO_CREATE_RES
};

///////////////////////////////////////////////////////////////////////////////

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

   DX9Settings( RenderingDevice& device );
};

///////////////////////////////////////////////////////////////////////////////

class DX9Renderer : public Renderer, 
                    public Subject<DX9Renderer, DX9GraphResourceOp>
{
private:
   IDirect3D9&                m_d3d9;
   IDirect3DDevice9*          m_d3Device;
   DX9Settings                m_settings;
   D3DPRESENT_PARAMETERS      m_creationParams;

   bool                       m_deviceLost;
   bool                       m_hardwareTLOn;
   unsigned int               m_maxLights;
   D3DVIEWPORT9               m_viewport;

   D3DFORMAT                  m_bestTextureFormats[ TU_MAX ];

public:
   /**
    * Constructor.
    */
   DX9Renderer( IDirect3D9& d3d9,
                IDirect3DDevice9* d3Device, 
                const DX9Settings& settings,
                D3DPRESENT_PARAMETERS creationParams,
                UINT frontBufferWidth,
                UINT frontBufferHeight,
                bool hardwareTLOn );
   ~DX9Renderer();

   /**
    * Returns the rendering device.
    */
   inline IDirect3DDevice9& getD3Device() { return *m_d3Device; }

   /**
    * Tells if the graphics adapter offers a hardware support for transformation & lighting
    */
   inline bool isHardwareTLOn() const { return m_hardwareTLOn; }

   /**
    * Returns the render back buffer width.
    */
   inline UINT getBackBufferWidth() const { return m_creationParams.BackBufferWidth; }

   /**
    * Returns the render back buffer height.
    */
   inline UINT getBackBufferHeight() const { return m_creationParams.BackBufferHeight; }

   /**
    * Gives access to the settings the renderer was created with.
    */
   inline const DX9Settings& getSettings() const { return m_settings; }

   /**
    * Creates a vertex buffer.
    *
    * @param length     (in bytes)
    * @param usageFlags
    * @param format
    * @param memoryPool
    */
   IDirect3DVertexBuffer9* createVertexBuffer( UINT length, DWORD usageFlags, DWORD fvf, D3DPOOL memoryPool );

   /**
    * Creates an index buffer.
    *
    * @param length     (in bytes)
    * @param usageFlags
    * @param format
    * @param memoryPool
    */
   IDirect3DIndexBuffer9* createIndexBuffer( UINT length, DWORD usageFlags, D3DFORMAT format, D3DPOOL memoryPool );

   /**
    * Finds an optimal format for a texture for the specified use case.
    *
    * @param usage
    */
   inline D3DFORMAT getOptimalTextureFormat( TextureUsage usage ) const { return m_bestTextureFormats[usage]; }

protected:
   void resetViewport(unsigned int width, unsigned int height);
   void renderingBegin();
   void renderingEnd();
   bool isGraphicsSystemReady() const;
   void attemptToRecoverGraphicsSystem();
   void activateRenderTarget( RenderTarget* renderTarget );
   void cleanRenderTarget( const Color& bgColor );

private:
   void initRenderer();
   void sampleOptimalTextureFormats();
};

///////////////////////////////////////////////////////////////////////////////
