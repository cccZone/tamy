#pragma once

/// @file   core-Renderer\Shader.h
/// @brief  shader abstraction

#include <core\Array.h>
#include "core\Resource.h"
#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"
#include <string>
#include <vector>
#include <map>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Texture;
class ShaderImpl;
class Filesystem;
class SingletonsManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * A shader class represents a program written in HLSL.
 */
class Shader : public TRendererObject<ShaderImpl>, public Resource
{
   DECLARE_RTTI_CLASS

private:
   std::string m_name;
   std::string m_script;

public:
   /**
    * Default constructor.
    */
   Shader();

   /**
    * Resource-compliant constructor.
    */
   Shader(Filesystem& fs, const std::string& fileName);

   /**
    * Constructor.
    *
    * @param name    name of the HLSL shader script file
    * @param script  HLSL script
    */
   Shader(const std::string& name, const std::string& script);
   virtual ~Shader();

   /**
    * Returns the HLSL script of this shader.
    *
    * @return  HLSL shader script
    */
   const std::string& getScript() const;

   /**
    * Returns the name of the shader script.
    */
   const std::string& getName() const;

   /**
    * Starts the shader rendering process.
    */
   unsigned int beginRendering();

   /** 
    * End the shader rendering process.
    */
   void endRendering();

   /** 
    * Start a single rendering pass.
    *
    * @param passIdx    index of the started rendering pass.
    */
   void beginPass(unsigned int passIdx);

   /** 
    * Finish a single rendering pass.
    *
    * @param passIdx    index of the finished rendering pass.
    */
   void endPass(const unsigned int& passIdx);

   // -------------------------------------------------------------------------
   // Shader program setters
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
   
   void setTexture(const std::string& paramName, Texture& val);
   
   void setVec4(const std::string& paramName, const D3DXVECTOR4& val);
   
   void setVec4(const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size);

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onLoaded(ResourcesManager& mgr);
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Library dependent shader implementation.
 */
class ShaderImpl : public RendererObjectImpl
{
public:
   virtual ~ShaderImpl() {}

   virtual void setTechnique(const std::string& techniqueName) {}
   virtual void setBool(const std::string& paramName, bool val) {}
   virtual void setInt(const std::string& paramName, int val) {}
   virtual void setInt(const std::string& paramName, const int* arr, unsigned int size) {}
   virtual void setFloat(const std::string& paramName, float val) {}
   virtual void setFloat(const std::string& paramName, const float* arr, unsigned int size) {}
   virtual void setMtx(const std::string& paramName, const D3DXMATRIX& val) {}
   virtual void setMtx(const std::string& paramName, const D3DXMATRIX* arr, unsigned int size) {}
   virtual void setString(const std::string& paramName, const std::string& val) {}
   virtual void setTexture(const std::string& paramName, Texture& val) {}
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
