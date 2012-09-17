/// @file   core-Renderer/CascadedShadowsUtils.h
/// @brief  a set of tools used to calculate a cascaded shadow map
#pragma once

#include "core/AABoundingBox.h"
#include "core/Matrix.h"
#include "core-Renderer/Viewport.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;
struct Vector;

///////////////////////////////////////////////////////////////////////////////

struct CascadesConfig
{
   // runtime data
   Camera*           m_activeCamera;
   Matrix            m_lightRotationMtx;
   AABoundingBox     m_sceneBounds;

   // static data
   int               m_numCascades;
   int               m_numCascadesPerTextureRow;
   float             m_pcfBlurSize;
   float             m_cascadeDimensions;
   float*            m_cascadeIntervals;

   CascadesConfig( int numCascades, int numCascadesPerTextureRow, float pcfBlurSize );
   ~CascadesConfig();
};

///////////////////////////////////////////////////////////////////////////////

struct CascadeStage
{
   Viewport             m_viewport;
   AABoundingBox        m_objectsQueryBounds;
   AABoundingBox        m_lightFrustumBounds;
   float                m_cameraNearZ;
   float                m_cameraFarZ;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A set of tools used to calculate a cascaded shadow map.
 */
class CascadedShadowsUtils
{
public:
   /**
    * Calculates the cascaded shadow rendering settings.
    *
    * @param config
    * @param outArrCascadeStages
    */
   static void calculateCascadesBounds( const CascadesConfig& config, CascadeStage* outArrCascadeStages );

private:
   static void calculateCameraCascadeFrustumBounds( Camera* activeCamera, float intervalBegin, float intervalEnd, AABoundingBox& outFrustumPart );

   static void createBBPoints( const AABoundingBox& inAABB, Vector* outBBPoints );

   static void calculateLightClippingPlanes( const Vector* sceneBBInLightSpace, AABoundingBox& inOutLightFrustumBounds );
};

///////////////////////////////////////////////////////////////////////////////
