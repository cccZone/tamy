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

   Material& getMaterial(DWORD subsetIdx) const;

   unsigned int getNumSubsets() const;

   /**
    * This method creates an instance of the GraphicalEntity
    */
   Node* instantiate();

   /**
    * @Inherited
    */
   virtual void render(DWORD subset) = 0;

protected:
   void accept(GraphicalEntityInstantiator& instantiator);
};

///////////////////////////////////////////////////////////////////////////////
