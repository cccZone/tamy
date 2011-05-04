#include "GeometryManualEditor.h"
#include "core-Renderer.h"
#include "core-MVC.h"


///////////////////////////////////////////////////////////////////////////////

GeometryManualEditor::GeometryManualEditor( Geometry& geometry )
   : NodeManualEditor( dynamic_cast< SpatialEntity& >( geometry.getParent() ) )
{

}

///////////////////////////////////////////////////////////////////////////////
