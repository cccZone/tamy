/// @file   core-Renderer/FragmentShader.h
/// @brief  shader functions library resource
#pragma once

#include "core/Resource.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class FragmentShader : public Resource
{
   DECLARE_RESOURCE( VertexShader )

private:
   std::string                   m_script;

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   FragmentShader( const FilePath& resourceName = FilePath() );

   /**
    * Returns the HLSL script of this shader.
    *
    * @return  HLSL shader script
    */
   inline const std::string& getScript() const { return m_script; }

   /**
    * Sets the new HLSL script for this shader.
    *
    * @param script
    */
   inline void setScript( const std::string& script ) { m_script = script; }
};

///////////////////////////////////////////////////////////////////////////////
