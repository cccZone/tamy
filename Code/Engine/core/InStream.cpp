#include "core/InStream.h"
#include "core/types.h"


///////////////////////////////////////////////////////////////////////////////

InStream& InStream::operator>>( std::string& val )
{
   // load the length of the string
   uint len;
   load( (void*)&len, sizeof( uint ) );

   // load its contents
   char* tmpStr = new char[len + 1];
   load( (void*)tmpStr, len );
   val = std::string( tmpStr );
   delete tmpStr;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////
