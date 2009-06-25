#pragma once

#include "core-ResourceManagement\MaterialStageFactory.h"
#include "core-Renderer\MaterialStage.h"
#include "MaterialOperationImplementationMock.h"


//////////////////////////////////////////////////////////////////////////////

struct Mock;

//////////////////////////////////////////////////////////////////////////////

MaterialStage* MaterialStageFactory<Mock>::operator()(Texture& texture,
               MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
               MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2)
{
   static MaterialOperationImplementationMock s_matOp;

   return new MaterialStage(texture, 
                        new MaterialOperation(s_matOp, colorOp, colorArg1, colorArg2),
                        new MaterialOperation(s_matOp, alphaOp, alphaArg1, alphaArg2));
}

//////////////////////////////////////////////////////////////////////////////
