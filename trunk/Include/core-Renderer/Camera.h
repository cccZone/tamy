#pragma once

#include "core\Node.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class ProjectionCalculator;

///////////////////////////////////////////////////////////////////////////////

/**
 * A camera is a kind of Node that allows to set 
 * camera specific stuff and represents a camera that watches the scene
 */
class Camera : public Node
{
private:
   float m_fov;
   float m_aspectRatio;
   float m_nearZPlane;
   float m_farZPlane;

   D3DXMATRIX m_mtxIdentity;
   D3DXMATRIX m_mtxView;
   D3DXMATRIX m_mtx3DProjection;

   bool m_mtxProjectionDirty;

   ProjectionCalculator* m_projCalc;

public:
   Camera(const std::string& name);
   ~Camera();

   void setProjectionCalculator(ProjectionCalculator* projCalc);

   const D3DMATRIX& getViewMtx();

   const D3DMATRIX& getProjectionMtx3D();

   void setNearPlaneDimensions(float width, float height);
   float getAspectRatio() const {return m_aspectRatio;}

   void setClippingPlanes(float nearZPlane, float farZPlane);
   float getNearClippingPlane() const {return m_nearZPlane;}
   float getFarClippingPlane() const {return m_farZPlane;}

   /**
    * Sets the fov of the camera (angle specified in degrees)
    */
   void setFOV(float angle);

   /**
    * Returns the current FOV angle of the camera (in degrees)
    */
   float getFOV() const;

protected:
   void onAccept(NodeVisitor& visitor);
   
private:
   void updateViewMtx();
   void updateProjectionMtx();
};

///////////////////////////////////////////////////////////////////////////////
