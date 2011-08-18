#pragma once

#include "core-Renderer\Renderer.h"
#include "core-Renderer\ShaderTexture.h"
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "core\Subject.h"
#include "dx9-Renderer\DX9EffectsResourcesStorage.h"
#include "dx9-Renderer\DX9FontResourcesStorage.h"
#include "dx9-Renderer\DX9LineSegmentsResourcesStorage.h"
#include "dx9-Renderer\DX9TriangleMeshResourcesStorage.h"
#include "dx9-Renderer\DX9PixelShaderResourcesStorage.h"
#include "dx9-Renderer\DX9SkeletonResourcesStorage.h"
#include "dx9-Renderer\DX9VertexShaderResourcesStorage.h"
#include "dx9-Renderer\DX9TextureResourcesStorage.h"
#include "dx9-Renderer/DX9RenderTargetResourcesStorage.h"


///////////////////////////////////////////////////////////////////////////////

enum VERTEXPROCESSING_TYPE;
struct RenderingDevice;
class EffectShader;

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

/**
 * A debug vertex definition.
 */
struct DebugVertex
{
   D3DXVECTOR3       m_vtx;
   DWORD             m_color;
};

///////////////////////////////////////////////////////////////////////////////

class DX9Renderer : public Renderer
{
private:
   IDirect3D9&                                     m_d3d9;
   IDirect3DDevice9*                               m_d3Device;
   DX9Settings                                     m_settings;
   D3DPRESENT_PARAMETERS                           m_creationParams;

   bool                                            m_deviceLost;
   bool                                            m_hardwareTLOn;
   unsigned int                                    m_maxLights;
   D3DVIEWPORT9                                    m_viewport;

   D3DFORMAT                                       m_bestTextureFormats[ TU_MAX ];

   // -------------------------------------------------------------------------
   // Resources
   // -------------------------------------------------------------------------
   FontStorage*                                    m_fonts;
   EffectsStorage*                                 m_effects;
   LineSegmentsStorage*                            m_lineSegments;
   TriangleMeshesStorage*                          m_triMeshes;
   PixelShadersStorage*                            m_pixelShaders;
   SkeletonsStorage*                               m_skeletons;
   VertexShadersStorage*                           m_vertexShaders;
   TexturesStorage*                                m_textures;
   RenderTargetsStorage*                           m_renderTargets;
   std::vector< IRenderResourceStorage* >          m_storages;

   // -------------------------------------------------------------------------
   // Debug draw data
   // -------------------------------------------------------------------------
   // some limit definitions
   const unsigned int                              DEBUG_LINES_MAX_COUNT;
   D3DXMATRIX                                      m_identityMtx;
   IDirect3DVertexBuffer9*                         m_linesBuffer;
   DebugVertex*                                    m_pVertex;
   unsigned int                                    m_linesCount;

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
   inline IDirect3DDevice9& getD3Device() const { return *m_d3Device; }

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
   IDirect3DVertexBuffer9* createVertexBuffer( UINT length, DWORD usageFlags, DWORD fvf, D3DPOOL memoryPool ) const;

   /**
    * Creates an index buffer.
    *
    * @param length     (in bytes)
    * @param usageFlags
    * @param format
    * @param memoryPool
    */
   IDirect3DIndexBuffer9* createIndexBuffer( UINT length, DWORD usageFlags, D3DFORMAT format, D3DPOOL memoryPool ) const;

   /**
    * Finds an optimal format for a texture for the specified use case.
    *
    * @param usage
    */
   inline D3DFORMAT getOptimalTextureFormat( TextureUsage usage ) const { return m_bestTextureFormats[usage]; }

   // -------------------------------------------------------------------------
   // Resources acquisition
   // -------------------------------------------------------------------------
   /**
    * Returns an implementation dedicated to the specified effect.
    *
    * @param shader
    */
   ID3DXEffect* getEffect( EffectShader& shader );

   /**
    * Returns an implementation dedicated to the specified font.
    *
    * @param font
    */
   ID3DXFont* getFont( Font& font );

   /**
    * Returns an implementation dedicated to the specified line segments resource.
    *
    * @param segments
    */
   DX9LineSegments* getLineSegments( LineSegments& segments );

   /**
    * Returns an implementation dedicated to the specified triangle mesh resource.
    *
    * @param mesh
    */
   DX9TriangleMesh* getTriangleMesh( TriangleMesh& mesh );

   /**
    * Returns an implementation dedicated to the specified pixel shader resource.
    *
    * @param shader
    */
   DX9PixelShader* getPixelShader( PixelShader& shader );

   /**
    * Returns a vertex buffer dedicated to the specified skeleton.
    *
    * @param skeleton
    */
   IDirect3DVertexBuffer9* getSkeletonVertexBuffer( Skeleton& skeleton );

   /**
    * Returns an implementation dedicated to the specified vertex shader resource.
    *
    * @param shader
    */
   DX9VertexShader* getVertexShader( VertexShader& shader );

   /**
    * Returns an implementation dedicated to the specified texture resource.
    *
    * @param texture
    */
   IDirect3DTexture9* getTexture( Texture& texture );

   /**
    * Returns an implementation dedicated to the specified render target resource.
    *
    * @param renderTarget
    */
   DX9RenderTarget* getRenderTarget( RenderTarget& renderTarget );

   // -------------------------------------------------------------------------
   // Scene rendering
   // -------------------------------------------------------------------------
   /**
    * A signal to start rendering the scene and do everything related to it.
    */
   void beginScene();

   /**
    * A signal to stop rendering the scene.
    */
   void endScene();

   // -------------------------------------------------------------------------
   // Debug scene rendering
   // -------------------------------------------------------------------------
   /**
    * Flush renders the contents of the debug scene.
    */
   void flushDebugScene();

   /**
    * Adds a debug line to the debug scene.
    *
    * @param start
    * @param end
    * @param color
    */
   void addDebugLine( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color );

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
