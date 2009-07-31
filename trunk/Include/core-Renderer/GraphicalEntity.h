#pragma once

#include "core-Renderer\LeafGraphicalEntity.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Material;

///////////////////////////////////////////////////////////////////////////////

/**
 * This node represents a graphical entity 
 * that can be rendered on a graphical device
 */
class GraphicalEntity : public LeafGraphicalEntity
{
private:
   std::vector<Material*> m_materials;

public:
   GraphicalEntity(const std::string& name,
                   const std::vector<Material*>& materials);
   virtual ~GraphicalEntity();

   /**
    * Each entity has some rendering material set with which it should be rendered.
    * This method returns a reference to it.
    */
   Material& getMaterial(DWORD subsetIdx) const;

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
