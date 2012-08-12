#include "core-Renderer/ShaderCompiler.h"


///////////////////////////////////////////////////////////////////////////////

bool ShaderCompiler::parseVertexShaderTechniques( const std::string& shaderCode, std::vector< std::string >& outTechniqueNames, std::vector< std::string >& outEntryFunctions )
{
   // reset error messages
   m_errorMsg = "";

   // parse the technique markers located in the script
   int startPos = 0;
   std::vector< std::string > functionDeclElems;
   while ( ( startPos = shaderCode.find( "<technique>", startPos ) ) != std::string::npos )
   {
      // found the first marker, so now look for the end marker
      // and once you find it, we'll get the length of the string the markers contain - the length
      // of our technique name

      startPos += 11; // 11 is the length of "<technique>" string

      // the next line should contain the function declaration - extract the function name and store it
      // as the technique's entry function
      int functionDeclStart = shaderCode.find_first_of( "\n", startPos );
      if ( functionDeclStart == std::string::npos )
      {
         m_errorMsg += "Function declaration not found\n";
         return false;
      }

      functionDeclStart += 1;
      int functionDeclEnd = shaderCode.find_first_of( "{", functionDeclStart );
      if ( functionDeclEnd == std::string::npos )
      {
         m_errorMsg += "Invalid function declaration encountered\n";
         return false;
      }

      std::string functionDecl = shaderCode.substr( functionDeclStart, functionDeclEnd - functionDeclStart );

      functionDeclElems.clear();
      StringUtils::tokenize( functionDecl," \t()", functionDeclElems );

      if ( functionDeclElems.size() < 2 )
      {
         m_errorMsg += "Invalid function declaration\n";
         return false;
      }

      // store the technique name
      outEntryFunctions.push_back( functionDeclElems[1] );
      outTechniqueNames.push_back( functionDeclElems[0] );

      // jump to the next position in the script string
      startPos = functionDeclEnd + 1;
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

std::string ShaderCompiler::parseRequiredVertexShaderTechnique( const std::string& shaderCode )
{
   // reset error messages
   m_errorMsg = "";

   // parse the technique markers located in the script
   std::vector< std::string > functionDeclElems;
   int startPos = shaderCode.find( "<psMain>", 0 );
   if ( startPos == std::string::npos )
   {
      return "";
   }
   // found the first marker, so now look for the end marker
   // and once you find it, we'll get the length of the string the markers contain - the length
   // of our technique name

   startPos += 8; // 8 is the length of "<psMain>" string

   // the next line should contain the function declaration - extract the function name and store it
   // as the technique's entry function
   int functionDeclStart = shaderCode.find_first_of( "\n", startPos );
   if ( functionDeclStart == std::string::npos )
   {
      m_errorMsg += "Function declaration not found\n";
      return false;
   }

   functionDeclStart += 1;
   int functionDeclEnd = shaderCode.find_first_of( "{", functionDeclStart );
   if ( functionDeclEnd == std::string::npos )
   {
      m_errorMsg += "Invalid function declaration encountered\n";
      return false;
   }

   std::string functionDecl = shaderCode.substr( functionDeclStart, functionDeclEnd - functionDeclStart );
   StringUtils::tokenize( functionDecl," \t(),:", functionDeclElems );
   if ( functionDeclElems.size() < 4 )
   {
      m_errorMsg += "Invalid function declaration\n";
      return false;
   }

   return functionDeclElems[2];
}

///////////////////////////////////////////////////////////////////////////////
