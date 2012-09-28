#include "RPBMaterialsDB.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBMaterialsDB );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< RPMaterialsDBNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBMaterialsDB::RPBMaterialsDB( RPMaterialsDBNode& node )
   : TGraphBlock< RPMaterialsDBNode, RenderingPipelineNode >( node )
{
   setCaption( "MaterialsDB" );
}

///////////////////////////////////////////////////////////////////////////////
