#pragma once

#include "Node.h"
#include "MatrixWriter.h"
#include <sstream>
#include <typeinfo>
#include <list>
#include <deque>
#include "AbstractGraphicalEntity.h"
#include "CompositeGraphicalEntity.h"
#include "GraphicalEntity.h"
#include "SkinnedGraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntityHierarchyWriter
{
public:
   std::string operator()(AbstractGraphicalEntity& entity)
   {
      std::list<AbstractGraphicalEntity*> hierarchy = linearizeHierarchy(entity);

      std::stringstream result;

      for (std::list<AbstractGraphicalEntity*>::iterator it = hierarchy.begin(); 
           it != hierarchy.end(); ++it)
      {
         writeSingle(result, **it);
         result << std::endl;
      }

      return result.str();
   }

private:
   std::list<AbstractGraphicalEntity*> linearizeHierarchy(AbstractGraphicalEntity& entity)
   {
      std::list<AbstractGraphicalEntity*> result;
      std::deque<AbstractGraphicalEntity*> entitiesQueue;
      entitiesQueue.push_back(&entity);

      while(entitiesQueue.size() > 0)
      {
         AbstractGraphicalEntity* currEntity = entitiesQueue.back();
         entitiesQueue.pop_back();

         result.push_back(currEntity);

         for (std::list<AbstractGraphicalEntity*>::const_iterator it = currEntity->getChildren().begin();
              it != currEntity->getChildren().end(); ++it)
         {
            entitiesQueue.push_back(*it);
         }
      }

      return result;
   }

   void writeSingle(std::ostream& stream, AbstractGraphicalEntity& entity)
   {
      stream << "type : " << typeid(entity).name() << 
                "; name : " << entity.getName() <<
                "; children count : " << entity.getChildren().size() <<
                "; local mtx : " << entity.getLocalMtx();
   }
};

///////////////////////////////////////////////////////////////////////////////
