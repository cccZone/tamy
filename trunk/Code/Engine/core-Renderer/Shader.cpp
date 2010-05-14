#include "core-Renderer\Shader.h"
#include "core-Renderer\Renderer.h"
#include "core\File.h"
#include "core\StreamBuffer.h"
#include "core\SingletonsManager.h"
#include "core\ResourcesManager.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( Shader, Resource, tfx, AM_BINARY )
   PROPERTY( "hlsl", std::string, m_script )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

Shader::Shader( const std::string& fileName )
: Resource( Filesystem::changeFileExtension( fileName, Shader::getExtension() ) )
, m_fileName( fileName )
{
}

///////////////////////////////////////////////////////////////////////////////

Shader::Shader( const std::string& name, const std::string& script )
: Resource( name )
, m_script(script)
{
}

///////////////////////////////////////////////////////////////////////////////

Shader::~Shader() 
{
}

///////////////////////////////////////////////////////////////////////////////

void Shader::onResourceLoaded( ResourcesManager& mgr )
{
   if ( m_script.empty() )
   {
      ASSERT( !m_fileName.empty(), "Neither .fx file nor a shader script specified" );
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

   Renderer& renderer = mgr.getInitializers().shared<Renderer>();
   renderer.implement<Shader>(*this);
}

///////////////////////////////////////////////////////////////////////////////

const std::string& Shader::getScript() const
{
   return m_script;
}

///////////////////////////////////////////////////////////////////////////////

unsigned int Shader::beginRendering()
{
   return impl().beginRendering();
}

///////////////////////////////////////////////////////////////////////////////

void Shader::endRendering()
{
   impl().endRendering();
}

///////////////////////////////////////////////////////////////////////////////

void Shader::beginPass(unsigned int passIdx)
{
   impl().beginPass(passIdx);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::endPass(const unsigned int& passIdx)
{
   impl().endPass(passIdx);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setTechnique(const std::string& techniqueName)
{
   impl().setTechnique(techniqueName);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setBool(const std::string& paramName, bool val)
{
   impl().setBool(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setInt(const std::string& paramName, int val)
{
   impl().setInt(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setInt(const std::string& paramName, const int* arr, unsigned int size)
{
   impl().setInt(paramName, arr, size);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setFloat(const std::string& paramName, float val)
{
   impl().setFloat(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setFloat(const std::string& paramName, const float* arr, unsigned int size)
{
   impl().setFloat(paramName, arr, size);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setMtx(const std::string& paramName, const D3DXMATRIX& val)
{
   impl().setMtx(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setMtx(const std::string& paramName, const D3DXMATRIX* arr, unsigned int size)
{
   impl().setMtx(paramName, arr, size);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setString(const std::string& paramName, const std::string& val)
{
   impl().setString(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setTexture(const std::string& paramName, Texture& val)
{
   impl().setTexture(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setVec4(const std::string& paramName, const D3DXVECTOR4& val)
{
   impl().setVec4(paramName, val);
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setVec4(const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size)
{
   impl().setVec4(paramName, arr, size);
}

///////////////////////////////////////////////////////////////////////////////