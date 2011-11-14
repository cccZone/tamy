#include "dx9-Renderer/DX9ShaderIncludeLoader.h"
#include "core-Renderer/Defines.h"
#include "core/ResourcesManager.h"
#include "core/Filesystem.h"
#include "core/File.h"
#include "core/StreamBuffer.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

HRESULT DX9ShaderIncludeLoader::Open( D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes )
{
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   std::string fileName( SHADERS_DIR );
   fileName += pFileName;

   ASSERT_MSG( !fileName.empty(), "No shader script filename specified" );
   if ( fileName.empty() )
   {
      return E_FAIL;
   }

   File* file = fs.open( fileName, std::ios_base::in | std::ios_base::binary );
   if ( file == NULL )
   {
      ASSERT_MSG( !fileName.empty(), "HLSL shader file doesn't exist" );
      return E_FAIL;
   }
   StreamBuffer<char> shaderScript( *file );

   unsigned int size = shaderScript.size();
   char* script = new char[ size + 1 ];
   strcpy( script, shaderScript.getBuffer() );
   delete file;

   *ppData = (LPCVOID)script;
   *pBytes = size;

   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////

HRESULT DX9ShaderIncludeLoader::Close( LPCVOID pData )
{
   char* str = (char*)pData;
   delete [] str;
   return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
