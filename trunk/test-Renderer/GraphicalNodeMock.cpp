#include "GraphicalNodeMock.h"
#include "core-Renderer\MaterialOperation.h"
#include "LightReflectingPropertiesStub.h"


///////////////////////////////////////////////////////////////////////////////

RegularTests::RenderingTargetsPolicyMock GraphicalNodeMock::s_policy;
MaterialOperationImplementationMock GraphicalNodeMock::s_matOpImpl;
TransparencyEnablerStub GraphicalNodeMock::s_transparencyEnabler;
Material GraphicalNodeMock::s_material("", s_policy, new LightReflectingPropertiesStub(), s_matOpImpl, s_matOpImpl, s_transparencyEnabler);

///////////////////////////////////////////////////////////////////////////////

namespace GraphicalEffectTests
{

RegularTests::RenderingTargetsPolicyMock GraphicalNodeMock::s_policy;
MaterialOperationImplementationMock GraphicalNodeMock::s_matOpImpl;
TransparencyEnablerStub GraphicalNodeMock::s_transparencyEnabler;
Material GraphicalNodeMock::s_material("", s_policy, new LightReflectingPropertiesStub(), s_matOpImpl, s_matOpImpl, s_transparencyEnabler);

}

///////////////////////////////////////////////////////////////////////////////