#include "dx9-Renderer/DX9ShaderIncludeLoader.h"
#include "core-Renderer/FragmentShader.h"
#include "core/ResourcesManager.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

HRESULT DX9ShaderIncludeLoader::Open( D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   FragmentShader& res = resMgr.create< FragmentShader >( FilePath( pFileName ) );

   const std::string& script = DynamicCast< FragmentShader >( &res )->getScript();

   *ppData = (LPCVOID)script.c_str();
   *pBytes = script.length();

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////

HRESULT DX9ShaderIncludeLoader::Close( LPCVOID pData )
{
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
