#ifndef _CORERENDERER_GRAPHICALEFFECT_H
#define _CORERENDERER_GRAPHICALEFFECT_H

#include <string>
#include <core\Array.h>
#include <vector>
#include <map>
#include <d3dx9.h>
#include "core-Renderer\RenderingTargetsPolicy.h"


///////////////////////////////////////////////////////////////////////////////

class Texture;
class RenderingTargetsPolicy;

///////////////////////////////////////////////////////////////////////////////

class GraphicalEffect
{
private:
   std::string m_name;

public:
   GraphicalEffect(const std::string& name);
   virtual ~GraphicalEffect();

   const std::string& getName() const {return m_name;}

   virtual void setTechnique(const std::string& material) = 0;
   virtual void setBool(const std::string& paramName, bool val) = 0;
   virtual void setInt(const std::string& paramName, int val) = 0;
   virtual void setInt(const std::string& paramName, const int* arr, unsigned int size) = 0;
   virtual void setFloat(const std::string& paramName, float val) = 0;
   virtual void setFloat(const std::string& paramName, const float* arr, unsigned int size) = 0;
   virtual void setMtx(const std::string& paramName, const D3DXMATRIX& val) = 0;
   virtual void setMtx(const std::string& paramName, const D3DXMATRIX* arr, unsigned int size) = 0;
   virtual void setString(const std::string& paramName, const std::string& val) = 0;
   virtual void setTexture(const std::string& paramName, Texture& val) = 0;
   virtual void setVec4(const std::string& paramName, const D3DXVECTOR4& val) = 0;
   virtual void setVec4(const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size) = 0;

   template <typename T>
   void render(T& renderable, RenderingTargetsPolicy& policy);

protected:
   /**
    * Provide the implementation specific code that starts the effect rendering process
    */
   virtual unsigned int beginRendering() = 0;

   /** 
    * @derrived
    *
    * This method signals the implementation that 
    * we finished the rendering process
    */
   virtual void endRendering() = 0;

   /** 
    * @derrived
    *
    * This method signals the implementation that 
    * we're starting a rendering pass
    */
   virtual void beginPass(const unsigned int& passIdx) = 0;

   /**
    * @derrived
    *
    * This method signals the implementation that 
    * we finished the rendering pass
    */
   virtual void endPass(const unsigned int& passIdx) = 0;
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\GraphicalEffect.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _CORERENDERER_GRAPHICALEFFECT_H
