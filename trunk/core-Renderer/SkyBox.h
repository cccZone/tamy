#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Material;

///////////////////////////////////////////////////////////////////////////////

enum SkyBoxSides
{
   SBS_FRONT,
   SBS_BACK,
   SBS_LEFT,
   SBS_RIGHT,
   SBS_TOP,
   SBS_BOTTOM
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a skybox - a background element
 * which adds a panoramic surrounding to the scene.
 */
class SkyBox
{
private:
   Material* m_materials[6];

public:
   SkyBox();
   virtual ~SkyBox() {}

   /**
    * A skybox is rendered relative to the position of the camera.
    * @param The active camera's world matrix
    */
   virtual void updateOrientation(const D3DXMATRIX& cameraWorldMtx) = 0;

   void render();

   void setMaterial(SkyBoxSides side, Material& material);

protected:
   /**
    * The method prepares the skybox for rendering - setting the
    * device etc.
    */
   virtual void startRendering() = 0;

   /**
    * The method renders the selected side of the sky box
    */
   virtual void renderSide(SkyBoxSides sideIdx) = 0;

   /**
    * The method restores the device to the state it was
    * before the skybox was rendered
    */
   virtual void endRendering() = 0;
};

///////////////////////////////////////////////////////////////////////////////
