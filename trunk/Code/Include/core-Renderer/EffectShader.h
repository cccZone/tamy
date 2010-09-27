#pragma once

/// @file   core-Renderer\EffectShader.h
/// @brief  effect shader abstraction

#include <core\Array.h>
#include "core\Resource.h"
#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"
#include <string>
#include <vector>
#include <map>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class ShaderTexture;
class EffectShaderImpl;
class Filesystem;
class SingletonsManager;
class GeometryResource;

///////////////////////////////////////////////////////////////////////////////

/**
 * A shader class represents a program written in HLSL.
 */
class EffectShader : public Resource, public TRendererObject<EffectShaderImpl>
{
   DECLARE_RESOURCE( EffectShader )

private:
   std::string m_fileName;
   std::string m_script;

public:
   /**
    * Constructor loading a shader from an .fx file
    *
    * @param fileName   name of a file with an HLSL shader
    */
   EffectShader( const std::string& fileName = "" );

   /**
    * Constructor.
    *
    * @param name    name for the resource
    * @param script  HLSL script
    */
   EffectShader( const std::string& name, const std::string& script );
   virtual ~EffectShader();

   /**
    * Returns the HLSL script of this shader.
    *
    * @return  HLSL shader script
    */
   const std::string& getScript() const;

   /**
    * Renders the geometry.
    *
    * @param geometry      geometry to render
    */
   void render( GeometryResource& geometry );

   // -------------------------------------------------------------------------
   // EffectShader program setters
   // -------------------------------------------------------------------------
   void setTechnique(const std::string& techniqueName);

   void setBool(const std::string& paramName, bool val);
   
   void setInt(const std::string& paramName, int val);
   
   void setInt(const std::string& paramName, const int* arr, unsigned int size);
   
   void setFloat(const std::string& paramName, float val);
   
   void setFloat(const std::string& paramName, const float* arr, unsigned int size);
   
   void setMtx(const std::string& paramName, const D3DXMATRIX& val);
   
   void setMtx(const std::string& paramName, const D3DXMATRIX* arr, unsigned int size);
   
   void setString(const std::string& paramName, const std::string& val);
   
   void setTexture(const std::string& paramName, ShaderTexture& val);
   
   void setVec4(const std::string& paramName, const D3DXVECTOR4& val);
   
   void setVec4(const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size);

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded(ResourcesManager& mgr);
   void onComponentAdded( Component< ResourcesManager >& component );
   void onComponentRemoved( Component< ResourcesManager >& component );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Library dependent shader implementation.
 */
class EffectShaderImpl : public RendererObjectImpl
{
public:
   virtual ~EffectShaderImpl() {}

   virtual void setTechnique(const std::string& techniqueName) {}
   virtual void setBool(const std::string& paramName, bool val) {}
   virtual void setInt(const std::string& paramName, int val) {}
   virtual void setInt(const std::string& paramName, const int* arr, unsigned int size) {}
   virtual void setFloat(const std::string& paramName, float val) {}
   virtual void setFloat(const std::string& paramName, const float* arr, unsigned int size) {}
   virtual void setMtx(const std::string& paramName, const D3DXMATRIX& val) {}
   virtual void setMtx(const std::string& paramName, const D3DXMATRIX* arr, unsigned int size) {}
   virtual void setString(const std::string& paramName, const std::string& val) {}
   virtual void setTexture(const std::string& paramName, ShaderTexture& val) {}
   virtual void setVec4(const std::string& paramName, const D3DXVECTOR4& val) {}
   virtual void setVec4(const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size) {}

   /**
    * Provide the implementation specific code that starts 
    * the effect rendering process.
    * 
    * @return  number of rendering passes in the script.
    */
   virtual unsigned int beginRendering() {return 0;}

   /** 
    * This method signals the implementation that 
    * we finished the rendering process.
    */
   virtual void endRendering() {}

   /** 
    * This method signals the implementation that 
    * we're starting a rendering pass.
    */
   virtual void beginPass(unsigned int passIdx) {}

   /**
    * This method signals the implementation that 
    * we finished the rendering pass.
    */
   virtual void endPass(unsigned int passIdx) {}
};

///////////////////////////////////////////////////////////////////////////////
