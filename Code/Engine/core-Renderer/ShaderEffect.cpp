#include "core-Renderer\ShaderEffect.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( ShaderEffect, Effect )
   PROPERTY_EDIT( "shader", Shader*, m_shader)
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

std::string ShaderEffect::getShaderName() const
{
   if ( m_shader )
   {
      return m_shader->getResourceName();
   }
   else
   {
      return "<<uninitialized shader effect>>";
   }
}

///////////////////////////////////////////////////////////////////////////////

void ShaderEffect::onPropertyChanged( Property& property )
{
   __super::onPropertyChanged( property );
}

///////////////////////////////////////////////////////////////////////////////

unsigned int ShaderEffect::beginRendering()
{
   if ( !m_shader )
   {
      return 0;
   }

   onBeginRendering();
   return m_shader->beginRendering();
}

///////////////////////////////////////////////////////////////////////////////

void ShaderEffect::beginPass(unsigned int passIdx)
{
   if ( !m_shader )
   {
      return;
   }

   onBeginPass(passIdx);
   m_shader->beginPass(passIdx);
}

///////////////////////////////////////////////////////////////////////////////

void ShaderEffect::endPass(unsigned int passIdx)
{
   if ( !m_shader )
   {
      return;
   }
   m_shader->endPass(passIdx);
   onEndPass(passIdx);
}

///////////////////////////////////////////////////////////////////////////////

void ShaderEffect::endRendering()
{
   if ( !m_shader )
   {
      return;
   }

   m_shader->endRendering();
   onEndRendering();
}

///////////////////////////////////////////////////////////////////////////////
