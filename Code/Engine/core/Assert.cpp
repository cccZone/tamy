#include "core/Assert.h"
#include <cassert>
#include <stdio.h>
#include <stdarg.h>


///////////////////////////////////////////////////////////////////////////////

void ASSERT( bool condition )
{
   assert( condition );
}

///////////////////////////////////////////////////////////////////////////////

void ASSERT( bool condition, const char* format, ... )
{
   if ( !condition )
   {
      va_list args;
      va_start( args, format );
      char msg[1024];
      vsprintf_s( msg, 1024, format, args );
      va_end( args );

      assert( condition && format );
   }
}

///////////////////////////////////////////////////////////////////////////////
