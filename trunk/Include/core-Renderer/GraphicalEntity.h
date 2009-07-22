#pragma once

#include "core-Renderer\LeafGraphicalEntity.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class RenderingTechnique;

///////////////////////////////////////////////////////////////////////////////

/**
 * This node represents a graphical entity 
 * that can be rendered on a graphical device
 */
class GraphicalEntity : public LeafGraphicalEntity
{
private:
   std::vector<RenderingTechnique*> m_techniques;

public:
   GraphicalEntity(const std::string& name,
                   const std::vector<RenderingTechnique*>& techniques);
   virtual ~GraphicalEntity();

   /**
    * Each entity has some rendering technique set with which it should be rendered.
    * This method returns a reference to it.
    */
   RenderingTechnique& getTechnique(DWORD subsetIdx) const;

   unsigned int getNumSubsets() const;

   /**
    * This method creates an instance of the GraphicalEntity
    */
   Node* instantiate(bool dynamic);

   /**
    * Renders the entity on the specified position
    */
   virtual void render(const D3DXMATRIX& globalMtx, DWORD subset) = 0;

protected:
   void accept(GraphicalEntityInstantiator& instantiator);
};

///////////////////////////////////////////////////////////////////////////////
