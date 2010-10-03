#include "core-Renderer\EffectShader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\GeometryResource.h"
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

void EffectShader::onComponentAdded( Component< ResourcesManager >& component )
{
   ResourceManagerComponent< Renderer >* rendererComp = dynamic_cast< ResourceManagerComponent< Renderer >* >( &component );
   if ( rendererComp )
   {
      rendererComp->get().implement< EffectShader >( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::onComponentRemoved( Component< ResourcesManager >& component )
{
   ResourceManagerComponent< Renderer >* rendererComp = dynamic_cast< ResourceManagerComponent< Renderer >* >( &component );
   if ( rendererComp )
   {
      setImplementation( NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////

const std::string& EffectShader::getScript() const
{
   return m_script;
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::render( GeometryResource& geometry )
{
   unsigned int count = impl().beginRendering();
   for ( unsigned int passIdx = 0; passIdx < count; ++passIdx )
   {
      impl().beginPass( passIdx );
      geometry.render();
      impl().endPass( passIdx );
   }
   impl().endRendering();
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setTechnique(const std::string& techniqueName)
{
   impl().setTechnique(techniqueName);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setBool(const std::string& paramName, bool val)
{
   impl().setBool(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setInt(const std::string& paramName, int val)
{
   impl().setInt(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setInt(const std::string& paramName, const int* arr, unsigned int size)
{
   impl().setInt(paramName, arr, size);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setFloat(const std::string& paramName, float val)
{
   impl().setFloat(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setFloat(const std::string& paramName, const float* arr, unsigned int size)
{
   impl().setFloat(paramName, arr, size);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setMtx(const std::string& paramName, const D3DXMATRIX& val)
{
   impl().setMtx(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setMtx(const std::string& paramName, const D3DXMATRIX* arr, unsigned int size)
{
   impl().setMtx(paramName, arr, size);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setString(const std::string& paramName, const std::string& val)
{
   impl().setString(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setTexture(const std::string& paramName, ShaderTexture& val)
{
   impl().setTexture(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setVec4(const std::string& paramName, const D3DXVECTOR4& val)
{
   impl().setVec4(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void EffectShader::setVec4(const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size)
{
   impl().setVec4(paramName, arr, size);
}

///////////////////////////////////////////////////////////////////////////////