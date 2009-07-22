#include "GraphicalEntityMock.h"
#include "core-Renderer\MaterialOperation.h"
#include "LightReflectingPropertiesStub.h"


///////////////////////////////////////////////////////////////////////////////

RegularTests::RenderingTargetsPolicyMock GraphicalEntityMock::s_policy;
MaterialOperationImplementationMock GraphicalEntityMock::s_matOpImpl;
TransparencyEnablerStub GraphicalEntityMock::s_transparencyEnabler;
Material GraphicalEntityMock::s_material("", s_policy, new LightReflectingPropertiesStub(), s_matOpImpl, s_matOpImpl, s_transparencyEnabler);

///////////////////////////////////////////////////////////////////////////////
