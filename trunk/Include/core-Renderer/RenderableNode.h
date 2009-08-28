#pragma once

#include "core\Node.h"
#include "core\Array.h"
#include "core-Renderer\Renderable.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Material;

///////////////////////////////////////////////////////////////////////////////

/**
 * A node that can be rendered on the screen
 */
class RenderableNode : public Node, public Renderable
{
private:
   Material* m_material;

public:
   virtual ~RenderableNode();

   /**
    * @inherited
    */
   virtual void render() = 0;

   /**
    * @inherited
    */
   virtual const Array<D3DXMATRIX>& getRenderingMatrices() = 0;

   /**
    * This method allows to set an arbitrary material that will be used
    * to render this node's contents
    */
   void setMaterial(Material& material);
   
   const Material& getMaterial() const;

   Material& getMaterial();

   const D3DXMATRIX& getGlobalTransform();

protected:
   RenderableNode(const std::string& name, Material& material);

   void onAccept(NodeVisitor& visitor);
};

///////////////////////////////////////////////////////////////////////////////
