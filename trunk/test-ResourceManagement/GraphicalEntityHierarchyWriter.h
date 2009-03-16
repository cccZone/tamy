#pragma once

#include "core\Node.h"
#include "core\MatrixWriter.h"
#include "core\HierarchyWriter.h"
#include <typeinfo>
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\CompositeGraphicalEntity.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\SkinnedGraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntityHierarchyWriter : public HierarchyWriter<AbstractGraphicalEntity>
{
protected:
   void writeSingle(std::ostream& stream, AbstractGraphicalEntity& entity)
   {
      stream << "type : " << typeid(entity).name() << 
                "; name : " << entity.getName() <<
                "; children count : " << entity.getChildren().size() <<
                "; local mtx : " << entity.getLocalMtx();
   }
};

///////////////////////////////////////////////////////////////////////////////
