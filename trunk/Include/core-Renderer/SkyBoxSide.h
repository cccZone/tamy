#pragma once

#include "core-Renderer\Renderable.h"
#include "core\BoundingSpace.h"


///////////////////////////////////////////////////////////////////////////////

class Texture;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a side of a skybox.
 */
class SkyBoxSide : public Renderable
{
private:
   Texture* m_texture;
   Material* m_material;
   
   D3DXMATRIX m_globalMtx;
   Array<D3DXMATRIX> m_renderingMatrices;
   BoundingSpace m_volume;

public:
   SkyBoxSide(Texture* texture);
   virtual ~SkyBoxSide();

   /**
    * @inherited
    */
   virtual void render() = 0;

   const BoundingVolume& getVolume();

   const Array<D3DXMATRIX>& getRenderingMatrices();
   
   const Material& getMaterial() const;

   const D3DXMATRIX& getGlobalTransform();

   /**
    * This method allows to set an alternative matrix
    * that will be used as the rendering matrix.
    *
    * @param mtx     new matrix we want to use as this object's
    *                rendering matrix
    */
   void setGlobalMtx(const D3DXMATRIX& mtx);
};

///////////////////////////////////////////////////////////////////////////////
