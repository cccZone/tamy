#pragma once

/// @file   TamyEditor/NodeManualEditor.h
/// @brief  A manual editor capable of changing spatial entity's transformation

#include "EntityManualEditor.h"


///////////////////////////////////////////////////////////////////////////////

class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * A manual editor capable of changing spatial entity's transformation.
 */
class NodeManualEditor : public EntityManualEditor
{
private:
   EditEntityCommand*   m_host;
   SpatialEntity&       m_node;

public:
   NodeManualEditor( SpatialEntity& node );

   // -------------------------------------------------------------------------
   // EntityManualEditor implementation
   // -------------------------------------------------------------------------
   void initialize( EditEntityCommand& host );
   void edit( const D3DXVECTOR2& valChange, const Camera& camera );
};

///////////////////////////////////////////////////////////////////////////////
