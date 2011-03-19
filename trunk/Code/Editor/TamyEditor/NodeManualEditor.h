#pragma once

/// @file   TamyEditor/NodeManualEditor.h
/// @brief  A manual editor capable of changing spatial entity's transformation

#include "EntityManualEditor.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class SpatialEntity;
class Gizmo;
class DebugScene;
class Camera;

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

   DebugScene*          m_debugScene;
   Gizmo*               m_gizmo;

public:
   NodeManualEditor( SpatialEntity& node );
   virtual ~NodeManualEditor();

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
