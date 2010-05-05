#include "core-Renderer\ShaderEffect.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( ShaderEffect, Effect )
   PROPERTY( "shader", Shader*, m_shader)
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

ShaderEffect::ShaderEffect()
: m_shader(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

ShaderEffect::~ShaderEffect()
{
   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ShaderEffect::initialize(Shader& shader)
{
   ASSERT(m_shader == NULL, "ShaderEffect already initialized");
   m_shader = &shader;
}

///////////////////////////////////////////////////////////////////////////////

unsigned int ShaderEffect::beginRendering()
{
   ASSERT(m_shader != NULL, "ShaderEffect instance not initialized");

   onBeginRendering();
   return m_shader->beginRendering();
}

///////////////////////////////////////////////////////////////////////////////

void ShaderEffect::beginPass(unsigned int passIdx)
{
   ASSERT(m_shader != NULL, "ShaderEffect instance not initialized");

   onBeginPass(passIdx);
   m_shader->beginPass(passIdx);
}

///////////////////////////////////////////////////////////////////////////////

void ShaderEffect::endPass(unsigned int passIdx)
{
   ASSERT(m_shader != NULL, "ShaderEffect instance not initialized");

   m_shader->endPass(passIdx);
   onEndPass(passIdx);
}

///////////////////////////////////////////////////////////////////////////////

void ShaderEffect::endRendering()
{
   ASSERT(m_shader != NULL, "ShaderEffect instance not initialized");

   m_shader->endRendering();
   onEndRendering();
}

///////////////////////////////////////////////////////////////////////////////
