#ifndef _SHADER_EFFECT_H
#error "This file can only be included in ShaderEffect.h"
#else

#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

Shader& ShaderEffect::shader()
{
   ASSERT(m_shader != NULL, "ShaderEffect instance not initialized");
   return *m_shader;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _SHADER_EFFECT_H
