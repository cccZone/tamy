#pragma once

#include "core-Renderer\TextureRenderingTarget.h"
#include "core\Observer.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

/**
 * This rendering target decorator will make sure
 * the targets it aggregates are changed cyclically
 * with each call to the @see use method
 *
 * It can also be used as a texture
 */
class CyclicRenderingTarget : public TextureRenderingTarget
{
private:
   std::list<TextureRenderingTarget*> m_targets;

public:
   CyclicRenderingTarget(const std::string& name);
   ~CyclicRenderingTarget();

   void add(TextureRenderingTarget* target);

   /**
    * This method switches the currently used buffer.
    */
   void switchBuffer();

   void use(unsigned char idx);

   /**
    * Returns the texture that contains the most recent 
    * rendering
    */
   TextureImpl& getImpl();

   /**
    * The list always contains the targets sorted in
    * descending order in terms of when the target was used
    * for rendering.
    * The head of the list contains the oldest targets,
    * and the tail - the ones that were used most recently.
    */
   const std::list<TextureRenderingTarget*>& getTargets() const {return m_targets;}
};

///////////////////////////////////////////////////////////////////////////////
