#include "GraphicalNodeMock.h"
#include "core-Renderer\MaterialOperation.h"
#include "LightReflectingPropertiesStub.h"


///////////////////////////////////////////////////////////////////////////////

MaterialOperationImplementationMock GraphicalNodeMock::s_matOpImpl;
TransparencyEnablerStub GraphicalNodeMock::s_transparencyEnabler;
Material GraphicalNodeMock::s_material("", new LightReflectingPropertiesStub(), s_matOpImpl, s_matOpImpl, s_transparencyEnabler);

///////////////////////////////////////////////////////////////////////////////
