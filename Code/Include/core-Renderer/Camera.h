/// @file   core-Renderer/Camera.h
/// @brief  3D camera
#pragma once

#include "core\MemoryRouter.h"
#include "core\Node.h"
#include "core\Observer.h"
#include "core\Frustum.h"
#include "core\Matrix.h"


///////////////////////////////////////////////////////////////////////////////

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
   DECLARE_ALLOCATOR( Camera, AM_ALIGNED_16 );

public:
   enum ProjectionType
   {
      PT_PERSPECTIVE,
      PT_ORTHO
   };

private:
   Renderer&            m_renderer;
   ProjectionType       m_projectionType;

   float                m_fov;
   float                m_aspectRatio;
   float                m_nearZPlane;
   float                m_farZPlane;
   float                m_nearPlaneWidth;
   float                m_nearPlaneHeight;

   Matrix               m_mtxView;
   Matrix               m_mtx3DProjection;

   bool                 m_mtxProjectionDirty;

public:
   /**
    * Constructor.
    *
    * @param name       camera name
    * @param renderer   main renderer the application uses
    */
   Camera( const std::string& name, Renderer& renderer, ProjectionType projectionType );
   ~Camera();

   /**
    * Sets a new projection type ( with perspective, orthogonal etc. )
    *
    * @param projectionType
    */
   void setProjection( ProjectionType projectionType );

   /**
    * Retrieves current view matrix.
    */
   const Matrix& getViewMtx();

   /**
    * Retrieves current projection matrix.
    */
   const Matrix& getProjectionMtx();

   /**
    * Sets new near plane dimensions
    *
    * @param width
    * @param height
    */
   void setNearPlaneDimensions( float width, float height );

   /**
    * Returns the dimensions of the new near plane
    *
    * @param outWidth
    * @param outHeight
    */
   void getNearPlaneDimensions( float& outWidth, float& outHeight ) const;

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

   /**
    * Sets the camera clipping planes.
    *
    * @param nearZPlane
    * @param farZPlane
    */
   void setClippingPlanes( float nearZPlane, float farZPlane );

   /**
    * Returns the camera clipping planes.
    *
    * @param outNearZPlane
    * @param outFarZPlane
    */
   void getClippingPlanes( float& outNearZPlane, float& outFarZPlane ) const;
   inline float getNearClippingPlane() const {return m_nearZPlane;}
   inline float getFarClippingPlane() const {return m_farZPlane;}


   /**
    * Sets the camera so that it looks at the specified node from
    * the specified distance.
    *
    * @param node
    * @param distance      ( must be >0 )
    */
   void lookAt( Node& node, const FastFloat& distance );

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
    *
    * @param outFrustum
    */
   void calculateFrustum( Frustum& outFrustum );

   /**
    * The method constructs a ray running from the camera global position
    * through the position on the viewport specified by the method params.
    *
    * The method params should be specified in the viewport coordinates (range <-1, 1>)
    *
    * @param viewportX
    * @param viewportY
    * @param outRay
    */
   void createRay( float viewportX, float viewportY, Ray& outRay );

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
