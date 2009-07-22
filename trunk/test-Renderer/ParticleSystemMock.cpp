#include "ParticleSystemMock.h"
#include "core-Renderer\MaterialOperation.h"
#include "LightReflectingPropertiesStub.h"


///////////////////////////////////////////////////////////////////////////////

RegularTests::RenderingTargetsPolicyMock ParticleSystemMock::s_policy;
MaterialOperationImplementationMock ParticleSystemMock::s_matOpImpl;
TransparencyEnablerStub ParticleSystemMock::s_transparencyEnabler;
Material ParticleSystemMock::s_material("", s_policy, new LightReflectingPropertiesStub(), s_matOpImpl, s_matOpImpl, s_transparencyEnabler);

///////////////////////////////////////////////////////////////////////////////
