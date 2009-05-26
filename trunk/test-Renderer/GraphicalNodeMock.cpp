#include "GraphicalNodeMock.h"
#include "core-Renderer\MaterialOperation.h"


///////////////////////////////////////////////////////////////////////////////

MaterialOperationImplementationMock GraphicalNodeMock::s_matOpImpl;
LightReflectingPropertiesStub GraphicalNodeMock::s_lrp;
Material GraphicalNodeMock::s_material(s_lrp, s_matOpImpl, s_matOpImpl);

///////////////////////////////////////////////////////////////////////////////
