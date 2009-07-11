#include "GraphicalEntityMock.h"
#include "core-Renderer\MaterialOperation.h"
#include "LightReflectingPropertiesStub.h"


///////////////////////////////////////////////////////////////////////////////

MaterialOperationImplementationMock GraphicalEntityMock::s_matOpImpl;
TransparencyEnablerStub GraphicalEntityMock::s_transparencyEnabler;
Material GraphicalEntityMock::s_material("", new LightReflectingPropertiesStub(), s_matOpImpl, s_matOpImpl, s_transparencyEnabler);

///////////////////////////////////////////////////////////////////////////////
