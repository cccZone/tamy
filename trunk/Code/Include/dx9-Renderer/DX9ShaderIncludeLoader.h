/// @file   dx9-Renderer/DX9ShaderIncludeLoader.h
/// @brief  implementation that allows to load included shader files
#pragma once

#include <d3d9.h>
#include <D3DX9Shader.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * Implementation that allows to load included shader files.
 */
class DX9ShaderIncludeLoader : public ID3DXInclude
{
public:
   // -------------------------------------------------------------------------
   // ID3DXInclude implementation
   // -------------------------------------------------------------------------
   HRESULT __stdcall  Open( D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes );
   HRESULT __stdcall  Close( LPCVOID pData );
};

///////////////////////////////////////////////////////////////////////////////
