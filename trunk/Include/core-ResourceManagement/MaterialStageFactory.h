#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"
#include "core-Renderer\MaterialOperation.h"


//////////////////////////////////////////////////////////////////////////////

class MaterialStage;
class Texture;
class ResourceManager;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<MaterialStage> : public AbstractResourceFactory
{
public:
   virtual ~Factory() {}

   virtual MaterialStage* operator()(Texture& texture,
                                     MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                     MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2) = 0;
};

//////////////////////////////////////////////////////////////////////////////

template<typename Impl>
class MaterialStageFactory : public Factory<MaterialStage>
{
protected:
   ResourceManager& m_resMgr;

public:
   MaterialStageFactory(ResourceManager& resMgr) : m_resMgr(resMgr) {}

   MaterialStage* operator()(Texture& texture,
                             MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                             MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2);
};

//////////////////////////////////////////////////////////////////////////////
