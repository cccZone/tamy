#pragma once

#include <string>
#include "core-Renderer\RenderingTechnique.h"
#include <core\Array.h>
#include <vector>
#include <map>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNode;
class EffectDataSource;
class Texture;

///////////////////////////////////////////////////////////////////////////////

class GraphicalEffect : public RenderingTechnique
{
private:
   EffectDataSource* m_dataSource;
   bool m_transparent;

public:
   GraphicalEffect(const std::string& name, 
                   RenderingTargetsPolicy& policy,
                   bool isTransparent,
                   EffectDataSource* dataSource);
   virtual ~GraphicalEffect();

   bool isTransparent() const {return m_transparent;}

   virtual void setTechnique(const std::string& technique) = 0;
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

protected:
   unsigned int  beginRendering();

   /**
    * Provide the implementation specific code that starts the effect rendering process
    */
   virtual unsigned int onBeginRendering() = 0;

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
