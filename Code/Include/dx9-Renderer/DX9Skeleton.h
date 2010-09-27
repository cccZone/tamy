#pragma once

#include "core-Renderer\Skeleton.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Skeleton : public SkeletonImpl
{
private:
   Skeleton&                     m_skeleton;
   IDirect3DDevice9*             m_d3Device;
   IDirect3DVertexBuffer9*       m_vb;

public:
   DX9Skeleton( Skeleton& skeleton );
   ~DX9Skeleton();

   // -------------------------------------------------------------------------
   // RendererObjectImpl implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );

   // -------------------------------------------------------------------------
   // SkeletonImpl implementation
   // -------------------------------------------------------------------------
   void setInStream();
};

///////////////////////////////////////////////////////////////////////////////
