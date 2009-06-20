#include "ParticleSystemMock.h"
#include "core-Renderer\MaterialOperation.h"


///////////////////////////////////////////////////////////////////////////////

MaterialOperationImplementationMock ParticleSystemMock::s_matOpImpl;
LightReflectingPropertiesStub ParticleSystemMock::s_lrp;
Material ParticleSystemMock::s_material(s_lrp, s_matOpImpl, s_matOpImpl);

///////////////////////////////////////////////////////////////////////////////
