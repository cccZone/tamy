#pragma once

#include "Node.h"
#include "MatrixWriter.h"
#include "HierarchyWriter.h"
#include <typeinfo>
#include "AbstractGraphicalEntity.h"
#include "CompositeGraphicalEntity.h"
#include "GraphicalEntity.h"
#include "SkinnedGraphicalEntity.h"


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
