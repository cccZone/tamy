#include "GraphicalNodeMock.h"
#include "core-Renderer\MaterialOperation.h"
#include "LightReflectingPropertiesStub.h"


///////////////////////////////////////////////////////////////////////////////

MaterialOperationImplementationMock GraphicalNodeMock::s_matOpImpl;
Material GraphicalNodeMock::s_material("", new LightReflectingPropertiesStub(), s_matOpImpl, s_matOpImpl);

///////////////////////////////////////////////////////////////////////////////
