#include "GraphicalEntityMock.h"
#include "core-Renderer\MaterialOperation.h"


///////////////////////////////////////////////////////////////////////////////

MaterialOperationImplementationMock GraphicalEntityMock::s_matOpImpl;
LightReflectingPropertiesStub GraphicalEntityMock::s_lrp;
Material GraphicalEntityMock::s_material(s_lrp, s_matOpImpl, s_matOpImpl);

///////////////////////////////////////////////////////////////////////////////
