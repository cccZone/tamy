#include "dx9-Renderer\DXErrorParser.h"


///////////////////////////////////////////////////////////////////////////////

std::string translateDxError( const std::string& desc, HRESULT errCode )
{
   switch( errCode )
   {
   case D3DERR_INVALIDCALL:
      {
         return desc + " due to invalid parameters passed";
         break;
      }

   case D3DERR_NOTAVAILABLE:
      {
         return desc + " - this format is unavailable";
         break;
      }

   case D3DERR_OUTOFVIDEOMEMORY:
      {
         return desc + " due to the lack of video memory";
         break;
      }

   case D3DXERR_INVALIDDATA:
      {
         return desc + " due to invalid data";
         break;
      }

   case E_OUTOFMEMORY:
      {
         return desc + " due to the lack of system memory";
         break;
      }

   default:
      {
         return desc + " for unknown reason";
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
