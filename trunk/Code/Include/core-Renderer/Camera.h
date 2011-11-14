/// @file   core-Renderer/Camera.h
/// @brief  3D camera
#pragma once

#include "core\Node.h"
#include <d3dx9.h>
#include "core\Observer.h"
#include "core\Frustum.h"


///////////////////////////////////////////////////////////////////////////////

class ProjectionCalculator;
struct Frustum;
struct Ray;
class Renderer;
enum RendererOps;

///////////////////////////////////////////////////////////////////////////////

/**
 * A camera is a kind of Node that allows to set 
 * camera specific stuff and represents a camera that watches the scene
 */
class Camera : public Node, public Observer<Renderer, RendererOps>
{
private:
   Renderer& m_renderer;

   float m_fov;
   float m_aspectRatio;
   float m_nearZPlane;
   float m_farZPlane;
   float m_nearPlaneWidth;
   float m_nearPlaneHeight;

   D3DXMATRIX m_mtxIdentity;
   D3DXMATRIX m_mtxView;
   D3DXMATRIX m_mtx3DProjection;

   bool m_mtxProjectionDirty;

   ProjectionCalculator* m_projCalc;

public:
   /**
    * Constructor.
    *
    * @param name       camera name
    * @param renderer   main renderer the application uses
    */
   Camera( const std::string& name, Renderer& renderer );
   ~Camera();

   /**
    * Sets a new projection calculator ( with perspective, orthogonal etc. )
    *
    * @param projCalc
    */
   void setProjectionCalculator( ProjectionCalculator* projCalc );

   /**
    * Retrieves current view matrix.
    */
   const D3DXMATRIX& getViewMtx();

   /**
    * Retrieves current projection matrix.
    */
   const D3DXMATRIX& getProjectionMtx();

   /**
    * Sets new near plane dimensions ( values in range (0,1) )
    *
    * @param width
    * @param height
    */
   void setNearPlaneDimensions( float width, float height );

   /**
    * Retrieves the width of the near plane.
    */
   inline float getNearPlaneWidth() const { return m_nearPlaneWidth; }

   /**
    * Retrieves the height of the near plane.
    */
   inline float getNearPlaneHeight() const { return m_nearPlaneHeight; }

   /**
    * Retrieves near plane aspect ration.
    */
   inline float getAspectRatio() const {return m_aspectRatio;}

   void setClippingPlanes(float nearZPlane, float farZPlane);
   inline float getNearClippingPlane() const {return m_nearZPlane;}
   inline float getFarClippingPlane() const {return m_farZPlane;}


   /**
    * Sets the camera so that it looks at the specified node from
    * the specified distance.
    *
    * @param node
    * @param distance      ( must be >0 )
    */
   void lookAt( Node& node, float distance );

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

   /**
    * This two method will be called whenever the renderer wishes to update 
    * its observers - and camera as an observer is attached to it
    */
   void update(Renderer& renderer);
   void update(Renderer& renderer, const RendererOps& operation);

protected:
   void onAccept(NodeVisitor& visitor);
   
private:
   void updateViewMtx();
   void updateProjectionMtx();
};

///////////////////////////////////////////////////////////////////////////////
