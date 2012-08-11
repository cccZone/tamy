#include "core-Renderer\VertexShader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\ShaderParam.h"
#include "core-Renderer\ShaderTexture.h"
#include "core-Renderer\ShaderCompiler.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( VertexShader, tvsh, AM_BINARY )
   PROPERTY( std::string, m_script )
   PROPERTY( std::string, m_entryFunctionName )
   PROPERTY( std::string, m_techniqueNames )
   PROPERTY( VertexDescId, m_vertexDescId )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

VertexShader::VertexShader( const FilePath& resourceName )
   : Resource( resourceName )
   , m_entryFunctionName( "main" )
   , m_vertexDescId( VDI_SIMPLE )
{
   m_arrEntryFunctionNames.push_back( m_entryFunctionName );
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::setVertexDescription( VertexDescId vertexDescId )
{
   m_vertexDescId = vertexDescId;
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< VertexShader >* VertexShader::createTextureSetter( const std::string& paramName, ShaderTexture& val )
{
   return val.createVertexShaderTextureSetter( paramName );
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::onObjectLoaded()
{
   __super::onObjectLoaded();

   // tokenize entry function names
   m_arrEntryFunctionNames.clear();
   StringUtils::tokenize( m_entryFunctionName, ";", m_arrEntryFunctionNames );

   // tokenize technique names
   m_arrTechniqueNames.clear();
   StringUtils::tokenize( m_techniqueNames, ";", m_arrTechniqueNames );
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::setScript( const std::string& script ) 
{ 
   m_script = script; 
   parseTechniques(); 
   setDirty(); 
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::parseTechniques()
{
   // clear previous list of technique entry functions
   m_arrEntryFunctionNames.clear();
   m_arrTechniqueNames.clear();
   m_entryFunctionName = "";
   m_techniqueNames = "";

   ShaderCompiler compiler;
   bool result = compiler.parseVertexShaderTechniques( m_script, m_arrTechniqueNames, m_arrEntryFunctionNames );
   if ( !result )
   {
      ASSERT_MSG( false, compiler.getLastError().c_str() );
   }

   if ( !result || m_arrEntryFunctionNames.empty() )
   {
      // if no technique markers were found or there was a parsing error, assume there's just one common technique 
      // that uses the 'main' method as its entry function

      m_entryFunctionName = "main";
      m_arrEntryFunctionNames.push_back( m_entryFunctionName );
   }
   else
   {
      // create the semicolon separated version of the arrays for serialization
      uint techniquesCount = m_techniqueNames.size();
      for ( uint i = 0; i < techniquesCount; ++i )
      {
         m_entryFunctionName += m_arrEntryFunctionNames[i] + ";";
         m_techniqueNames += m_arrTechniqueNames[i] + ";";
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
