#include "dx9-Renderer\DXErrorParser.h"
#include "core\types.h"


///////////////////////////////////////////////////////////////////////////////

std::string translateDxError( const std::string& desc, HRESULT errCode )
{
   switch( errCode )
   {
   case D3DERR_INVALIDCALL:
      {
         return desc + " due to invalid parameters passed";
      }

   case D3DERR_DEVICELOST:
      {
         return desc + " because the device is lost";
      }

   case D3DERR_NOTAVAILABLE:
      {
         return desc + " - this format is unavailable";
      }

   case D3DERR_OUTOFVIDEOMEMORY:
      {
         return desc + " due to the lack of video memory";
      }

   case D3DXERR_INVALIDDATA:
      {
         return desc + " due to invalid data";
      }

   case E_OUTOFMEMORY:
      {
         return desc + " due to the lack of system memory";
      }

   default:
      {
         long winErrorCode = HRESULT_CODE( errCode );
         LPVOID systemErrMsg, hModuleErrMsg;
         DWORD langId = MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT);
         FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, winErrorCode, langId, (LPTSTR)&systemErrMsg, 0, NULL ); 
         FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, winErrorCode, langId, (LPTSTR)&hModuleErrMsg, 0, NULL ); 

         char msg[1024];
         sprintf_s( msg, " [system: %s ] [hModule: %s] [code: %ld]", (LPTSTR)systemErrMsg, (LPTSTR)hModuleErrMsg, winErrorCode );
         LocalFree( systemErrMsg );
         LocalFree( hModuleErrMsg );

         return desc + msg;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
