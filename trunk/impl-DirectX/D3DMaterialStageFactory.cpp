#include "core-ResourceManagement\MaterialStageFactory.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "core-Renderer\MaterialStage.h"
#include "impl-DirectX\D3DColorOperationImplementation.h"
#include "impl-DirectX\D3DAlphaOperationImplementation.h"
#include <d3d9.h>


//////////////////////////////////////////////////////////////////////////////

MaterialStage* MaterialStageFactory<IDirect3D9>::operator()(
                  Texture& texture,
                  MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                  MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2)
{
   return new MaterialStage(texture,
            new MaterialOperation(m_resMgr.shared<D3DColorOperationImplementation>(), 
                                  colorOp, colorArg1, colorArg2),
            new MaterialOperation(m_resMgr.shared<D3DAlphaOperationImplementation>(), 
                                  alphaOp, alphaArg1, alphaArg2));
}

//////////////////////////////////////////////////////////////////////////////
