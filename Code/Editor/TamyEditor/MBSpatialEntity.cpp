#include "MBSpatialEntity.h"
#include "core-Renderer/MNSpatialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MBSpatialEntity, GraphBlock );
PROPERTY( TResourceHandle< MNSpatialEntity >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MBSpatialEntity::MBSpatialEntity( MNSpatialEntity& node )
   : TPipelineBlock< MNSpatialEntity, MaterialNode >( node )
{
   setCaption( "Spatial entity" );
}

///////////////////////////////////////////////////////////////////////////////
