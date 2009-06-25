#include "GraphicalEntityMock.h"
#include "core-Renderer\MaterialOperation.h"
#include "LightReflectingPropertiesStub.h"


///////////////////////////////////////////////////////////////////////////////

MaterialOperationImplementationMock GraphicalEntityMock::s_matOpImpl;
Material GraphicalEntityMock::s_material("", new LightReflectingPropertiesStub(), s_matOpImpl, s_matOpImpl);

///////////////////////////////////////////////////////////////////////////////
