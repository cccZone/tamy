#pragma once

#include "core-ResourceManagement\MaterialFactory.h"
#include "core-Renderer\Material.h"
#include "MaterialOperationImplementationMock.h"


//////////////////////////////////////////////////////////////////////////////

struct Mock;

//////////////////////////////////////////////////////////////////////////////

Material* MaterialFactory<Mock>::create(const std::string& name,
                                        unsigned int index, 
                                        LightReflectingProperties* lrp)
{
   static MaterialOperationImplementationMock s_matOpImpl;

   return new Material(name, lrp, s_matOpImpl, s_matOpImpl, index);
}

//////////////////////////////////////////////////////////////////////////////
