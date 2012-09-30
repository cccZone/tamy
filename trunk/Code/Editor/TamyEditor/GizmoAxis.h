/// @file   TamyEditor/GizmoAxis.h
/// @brief  a single axis of the nodes manipulation gizmo
#pragma once

#include "core-Renderer/Geometry.h"
#include "core/BoundingSpace.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;
class VertexShader;

///////////////////////////////////////////////////////////////////////////////

class GizmoOperation
{
public:
   virtual ~GizmoOperation() {}

   /**
    * Transforms the manipulated nodes using the specified transformation vector.
    *
    * @param manipulationAxis
    * @param transformationValue
    * @param outTransformationMtx
    */
   virtual void transformManipulatedNodes( const Vector& manipulationAxis, float transformationValue, Matrix& outTransformationMtx ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A single axis of the nodes manipulation gizmo.
 */
class GizmoAxis : public Geometry, public TimeDependent
{
   DECLARE_CLASS()

private:
   byte                             m_axisIdx;
   SpatialEntity&                   m_editedNode;
   Camera&                          m_activeCamera;
   GizmoOperation*                  m_operation;
   VertexShader*                    m_vertexShader;

public:
   /**
    * Constructor.
    *
    * @param axisIdx
    * @param editedNode
    * @param activeCamera
    */
   GizmoAxis( byte axisIdx, SpatialEntity& editedNode, Camera& activeCamera );
   ~GizmoAxis();

   /**
    * Sets an operation the gizmo axis should perform.
    */
   inline void setOperation( GizmoOperation& operation ) { m_operation = &operation; }

   /**
    * Transforms the manipulated nodes using the specified transformation vector.
    *
    * @param viewportSpaceTransformation
    */
   void transformManipulatedNodes( const Vector& viewportSpaceTransformation );

   /**
    * Updates axis' transform.
    */
   void update( float timeElapsed );

   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   RCBindVertexShader* onPreRender( Renderer& renderer );
   void onPostRender( Renderer& renderer );

private:
   void initialize();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * When customized with this operation, gizmo axis will translate the edited objects.
 */
class TranslationGizmoOp : public GizmoOperation
{
public:
   void transformManipulatedNodes( const Vector& manipulationAxis, float transformationValue, Matrix& outTransformationMtx ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * When customized with this operation, gizmo axis will translate the edited objects.
 */
class RotationGizmoOp : public GizmoOperation
{
public:
   void transformManipulatedNodes( const Vector& manipulationAxis, float transformationValue, Matrix& outTransformationMtx ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * When customized with this operation, gizmo axis will translate the edited objects.
 */
class ScalingGizmoOp : public GizmoOperation
{
public:
   void transformManipulatedNodes( const Vector& manipulationAxis, float transformationValue, Matrix& outTransformationMtx ) const;
};

///////////////////////////////////////////////////////////////////////////////
