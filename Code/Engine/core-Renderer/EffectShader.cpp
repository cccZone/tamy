#include "core-Renderer\EffectShader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\ShaderParam.h"
#include "core-Renderer\ShaderTexture.h"
#include "core.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( EffectShader, tfx, AM_BINARY )
   PROPERTY( std::string, m_script )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

EffectShader::EffectShader(const FilePath& resourceName )
   : Resource( resourceName )
{
}

///////////////////////////////////////////////////////////////////////////////

EffectShader::~EffectShader() 
{
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setScript( const std::string& script )
{
   m_script = script;
   setDirty();
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< EffectShader >* EffectShader::createTextureSetter( const std::string& paramName, ShaderTexture& val )
{
   return val.createEffectShaderTextureSetter( paramName );
}

///////////////////////////////////////////////////////////////////////////////
