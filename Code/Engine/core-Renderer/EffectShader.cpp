#include "core-Renderer\EffectShader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\ShaderParam.h"
#include "core-Renderer\ShaderTexture.h"
#include "core.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( EffectShader, Resource, tfx, AM_BINARY )
   PROPERTY_EDIT( "hlsl", std::string, m_script )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

EffectShader::EffectShader( const std::string& fileName )
: Resource( fileName )
, m_fileName( fileName )
{
}

///////////////////////////////////////////////////////////////////////////////

EffectShader::EffectShader( const std::string& name, const std::string& script )
: Resource( name )
, m_script(script)
{
}

///////////////////////////////////////////////////////////////////////////////

EffectShader::~EffectShader() 
{
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::onResourceLoaded( ResourcesManager& mgr )
{
   if ( m_script.empty() )
   {
      ASSERT_MSG( !m_fileName.empty(), "Neither .fx file nor a shader script specified" );
      if ( m_fileName.empty() )
      {
         return;
      }

      const Filesystem& fs = mgr.getFilesystem();
      File* file = fs.open( m_fileName, std::ios_base::in | std::ios_base::binary );
      if (file == NULL)
      {
         throw std::invalid_argument("HLSL file doesn't exist");
      }
      StreamBuffer<byte> buf(*file);

      StreamBuffer<char> shaderScript(*file);
      m_script = shaderScript.getBuffer();
      delete file;
   }
}

///////////////////////////////////////////////////////////////////////////////

const std::string& EffectShader::getScript() const
{
   return m_script;
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< EffectShader >* EffectShader::createTextureSetter( const std::string& paramName, ShaderTexture& val )
{
   return val.createEffectShaderTextureSetter( paramName );
}

///////////////////////////////////////////////////////////////////////////////
