/// @file   core-Renderer/VertexShaderConfigurator.h
/// @brief  external vertex shader configuration tool
#pragma once


///////////////////////////////////////////////////////////////////////////////

class RCBindVertexShader;

///////////////////////////////////////////////////////////////////////////////

class VertexShaderConfigurator
{
public:
   virtual ~VertexShaderConfigurator() {}

   /**
    * Configures the specified command.
    *
    * @param geometry
    * @param command
    */
   virtual void configure( const Geometry& geometry, RCBindVertexShader* command ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
