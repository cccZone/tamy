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
   PROPERTY( std::vector< ShaderConstantDesc >, m_constantsDescriptions )
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

ShaderParam< VertexShader >* VertexShader::createTextureSetter( MemoryPoolAllocator& allocator, const IDString& paramName, ShaderTexture& val )
{
   return val.createVertexShaderTextureSetter( allocator, paramName );
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::onObjectLoaded()
{
   __super::onObjectLoaded();

   // tokenize entry function names
   m_arrEntryFunctionNames.clear();
   StringUtils::tokenize( m_entryFunctionName, ";", m_arrEntryFunctionNames );

   // tokenize technique names
   m_arrTechniqueIds.clear();
   std::vector< std::string > arrTechniqueNames;
   StringUtils::tokenize( m_techniqueNames, ";", arrTechniqueNames );

   uint techniquesCount = arrTechniqueNames.size();
   for ( uint i = 0; i < techniquesCount; ++i )
   {
      m_arrTechniqueIds.push_back( generateTechniqueId( arrTechniqueNames[i] ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::setScript( const std::string& script ) 
{ 
   m_script = script; 
   parseTechniques();
   parseConstants();
   setDirty(); 
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::parseTechniques()
{
   // clear previous list of technique entry functions
   m_arrEntryFunctionNames.clear();
   m_arrTechniqueIds.clear();
   m_entryFunctionName = "";
   m_techniqueNames = "";

   ShaderCompiler compiler;
   std::vector< std::string > arrTechniqueNames;
   bool result = compiler.parseVertexShaderTechniques( m_script, arrTechniqueNames, m_arrEntryFunctionNames );
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
      uint techniquesCount = arrTechniqueNames.size();
      for ( uint i = 0; i < techniquesCount; ++i )
      {
         m_entryFunctionName += m_arrEntryFunctionNames[i] + ";";
         m_techniqueNames += arrTechniqueNames[i] + ";";

         m_arrTechniqueIds.push_back( generateTechniqueId( arrTechniqueNames[i] ) );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

uint VertexShader::generateTechniqueId( const std::string& techqniueName )
{
   if ( techqniueName.empty() )
   {
      return 0;
   }
   else
   {
      return StringUtils::calculateHash( techqniueName );
   }
}

///////////////////////////////////////////////////////////////////////////////

int VertexShader::findTechnique( uint techniqueId ) const
{
   uint count = m_arrTechniqueIds.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_arrTechniqueIds[i] == techniqueId )
      {
         return i;
      }
   }

   return -1;
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::parseConstants()
{
   m_constantsDescriptions.clear();
   ShaderCompiler compiler;

   // for each technique, collect the constants, and then merge them into one large list of unique constants
   uint techniquesCount = m_arrEntryFunctionNames.size();
   for ( uint techniqueIdx = 0; techniqueIdx < techniquesCount; ++techniqueIdx )
   {
      std::vector< ShaderConstantDesc > newConstants;
      compiler.compileVertexShaderConstants( m_script, m_arrEntryFunctionNames[techniqueIdx].c_str(), newConstants );

      uint newConstantsCount = newConstants.size();
      uint allConstantsCount = m_constantsDescriptions.size();
      for ( uint newIdx = 0; newIdx < newConstantsCount; ++newIdx )
      {
         bool exists = false;
         const ShaderConstantDesc& testedConstant = newConstants[newIdx];

         for ( uint allIdx = 0; allIdx < allConstantsCount; ++allIdx )
         {
            if ( testedConstant == m_constantsDescriptions[allIdx] )
            {
               // the same constant is already on our list
               exists = true;
               break;
            }
         }

         if ( !exists )
         {
            // this is a brand new constant
            m_constantsDescriptions.push_back( testedConstant );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RCBindVertexShader::RCBindVertexShader( VertexShader& shader, Renderer& renderer )
   : ShaderRenderCommand< VertexShader >( *renderer.getAllocator() )
   , m_shader( shader )
{
   uint techniqueId = renderer.getVertexShaderTechnique();
   if ( techniqueId != 0 )
   {
      m_techniqueIdx = shader.findTechnique( techniqueId );
      ASSERT_MSG( m_techniqueIdx >= 0, "This vertex shader does not implement the required technique. Default technique will be used" );
      if ( m_techniqueIdx < 0 )
      {
         m_techniqueIdx = 0;
      }
   }
   else
   {
      // use default technique, because no specific technique is required
      m_techniqueIdx = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
