#include "GBSpatialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GBSpatialEntity );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< GNSpatialEntity >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GBSpatialEntity::GBSpatialEntity( GNSpatialEntity& node )
   : TGraphBlock< GNSpatialEntity, GeometryShaderNode >( node )
{
   setCaption( "Spatial entity" );
}

///////////////////////////////////////////////////////////////////////////////
