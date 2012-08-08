#include "core-Renderer/ShaderTexture.h"

///////////////////////////////////////////////////////////////////////////////

BEGIN_ENUM( TextureUsage );
   ENUM_VAL( TU_COLOR );
   ENUM_VAL( TU_DEPTH );
   ENUM_VAL( TU_LUMINANCE );
   ENUM_VAL( TU_HDR );
   ENUM_VAL( TU_DATA_TEXTURE );
   ENUM_VAL( TU_32BIT_FP );
   ENUM_VAL( TU_COLOR_UNCOMPRESSED );
   ENUM_VAL( TU_COLOR_COMPRESSED );
END_ENUM();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( ShaderTexture );
   PARENT( ReflectionObject );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////