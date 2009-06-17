#pragma once

#include "core\Node.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class ProjectionCalculator;
struct Frustum;
struct Ray;

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

   const D3DMATRIX& getProjectionMtx();

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

   /**
    * The method returns the current camera frustum (in global coordinates)
    * The planes in the frustum ARE NORMALIZED
    */
   Frustum getFrustum();

   /**
    * The method constructs a ray running from the camera global position
    * through the position on the viewport specified by the method params.
    *
    * The method params should be specified in the viewport coordinates (range <-1, 1>)
    */
   Ray createRay(float viewportX, float viewportY);

protected:
   void onAccept(NodeVisitor& visitor);
   
private:
   void updateViewMtx();
   void updateProjectionMtx();
};

///////////////////////////////////////////////////////////////////////////////
