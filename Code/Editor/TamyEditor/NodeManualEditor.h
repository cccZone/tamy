#pragma once

/// @file   TamyEditor/NodeManualEditor.h
/// @brief  A manual editor capable of changing spatial entity's transformation

#include "EntityManualEditor.h"
#include "DebugRenderer.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class SpatialEntity;
class Gizmo;

///////////////////////////////////////////////////////////////////////////////

/**
 * A manual editor capable of changing spatial entity's transformation.
 */
class NodeManualEditor : public EntityManualEditor
{
private:
   EditEntityCommand*   m_host;
   SpatialEntity&       m_node;
   Camera*              m_camera;

   D3DXVECTOR3          m_rotationAxis;

   DebugRenderer*       m_debugRenderer;
   Gizmo*               m_gizmo;
   DebugHandle          m_gizmoRenderID;

public:
   NodeManualEditor( SpatialEntity& node );
   ~NodeManualEditor();

   // -------------------------------------------------------------------------
   // EntityManualEditor implementation
   // -------------------------------------------------------------------------
   void initialize( EditEntityCommand& host );
   void notifyEditModeChange();
   void startEdition( const D3DXVECTOR2& initClickPos );
   void edit( const D3DXVECTOR2& valChange );

private:
   void regenerateVectors();
   D3DXVECTOR3 calculateRotationAxis() const;
};

///////////////////////////////////////////////////////////////////////////////
