#pragma once

#include <string>
#include "core\Array.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Renderable;
class RenderingTarget;
class RenderingTargetsPolicy;

///////////////////////////////////////////////////////////////////////////////

class RenderingTechnique
{
private:
   static unsigned int m_nextIndex;

   std::string m_name;
   unsigned int m_index;
   RenderingTargetsPolicy& m_renderingTargetsPolicy;

   std::vector<std::vector<RenderingTarget*> > m_renderTargets;

public:
   RenderingTechnique(const std::string& name, RenderingTargetsPolicy& policy);
   RenderingTechnique(const RenderingTechnique& rhs);
   virtual ~RenderingTechnique() {}

   /**
    * RenderingTechnique bears a name. It gives it a 'recognizable label'.
    *
    * However - the name of the technique is not it's unique ID - it's index is.
    * Two completely different techniques can bear the same name, however
    * it is guaranteed that THEY WILL HAVE DIFFERENT IDs.
    */
   const std::string& getName() const {return m_name;}

   /** 
    * Since techinques are used in different rendering strategies,
    * we want to have a way of sorting through them.
    * Each technique can get an id assigned telling its order
    * That way we can use them in the sorting algorithms
    */
   unsigned int getIndex() const {return m_index;}
   bool operator<(const RenderingTechnique& rhs) const {return m_index < rhs.m_index;}

   /** 
    * Operator returns true if both techniques have the same id
    */
   bool operator==(const RenderingTechnique& rhs) const {return m_index == rhs.m_index;}
   bool operator!=(const RenderingTechnique& rhs) const {return m_index != rhs.m_index;}

   /**
    * This method is used to determine if the technique produces transparent looking
    * geometry (which needs to be rendered differently from the opaque geometry)
    */
   virtual bool isTransparent() const = 0;

   /** 
    * This method renders a group of nodes using this technique
    */
   void render(Array<Renderable*>& renderables);

protected:
   /** 
    * This method signals the implementation that 
    * we're starting the rendering process
    */
   virtual unsigned int beginRendering() = 0;

   /** 
    * This method signals the implementation that 
    * we finished the rendering process
    */
   virtual void endRendering() = 0;

   /** 
    * This method signals the implementation that 
    * we're starting a rendering pass
    */
   virtual void beginPass(const unsigned int& passIdx) = 0;

   /** 
    * This method signals the implementation that 
    * we finished the rendering pass
    */
   virtual void endPass(const unsigned int& passIdx) = 0;
};

///////////////////////////////////////////////////////////////////////////////
