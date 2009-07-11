#include "ParticleSystemMock.h"
#include "core-Renderer\MaterialOperation.h"
#include "LightReflectingPropertiesStub.h"


///////////////////////////////////////////////////////////////////////////////

MaterialOperationImplementationMock ParticleSystemMock::s_matOpImpl;
TransparencyEnablerStub ParticleSystemMock::s_transparencyEnabler;
Material ParticleSystemMock::s_material("", new LightReflectingPropertiesStub(), s_matOpImpl, s_matOpImpl, s_transparencyEnabler);

///////////////////////////////////////////////////////////////////////////////
