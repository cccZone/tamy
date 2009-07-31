#pragma once

#include <string>
#include <core\Array.h>
#include <vector>
#include <map>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Renderable;
class EffectDataSource;
class Texture;
class RenderingTargetsPolicy;

///////////////////////////////////////////////////////////////////////////////

class GraphicalEffect
{
private:
   std::string m_name;
   RenderingTargetsPolicy& m_renderingTargetsPolicy;
   EffectDataSource* m_dataSource;

public:
   GraphicalEffect(const std::string& name, 
                   RenderingTargetsPolicy& policy,
                   EffectDataSource* dataSource);
   virtual ~GraphicalEffect();

   const std::string& getName() const {return m_name;}

   virtual void setTechnique(const std::string& material) = 0;
   virtual void set(const std::string& paramName, bool val) = 0;
   virtual void set(const std::string& paramName, int val) = 0;
   virtual void set(const std::string& paramName, const Array<int>& val) = 0;
   virtual void set(const std::string& paramName, float val) = 0;
   virtual void set(const std::string& paramName, const Array<float>& val) = 0;
   virtual void set(const std::string& paramName, const D3DXMATRIX& val) = 0;
   virtual void set(const std::string& paramName, const Array<D3DXMATRIX>& val) = 0;
   virtual void set(const std::string& paramName, const std::string& val) = 0;
   virtual void set(const std::string& paramName, Texture& val) = 0;
   virtual void set(const std::string& paramName, const D3DXVECTOR4& val) = 0;
   virtual void set(const std::string& paramName, const Array<D3DXVECTOR4>& val) = 0;

   void render(Renderable& renderables);

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
