#pragma once

#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\Material.h"
#include <list>
#include <string>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntityMock : public GraphicalEntity
{
private:
   static Material s_material;
   std::vector<std::string>* m_messageSink;

public:
   GraphicalEntityMock(const std::string& id, 
                       const std::vector<Material*>& materials, 
                       std::vector<std::string>& messageSink) 
         : GraphicalEntity(id, materials),
         m_messageSink(&messageSink)
   {
   }

   GraphicalEntityMock(const std::string& id, 
                       const std::vector<Material*>& materials) 
         : GraphicalEntity(id, materials),
         m_messageSink(NULL)
   {
   }

   void render(DWORD subset)
   {
      if (m_messageSink != NULL)
      {
         std::stringstream msg;
         msg << "Render " << getName() << " subset " << subset;
         m_messageSink->push_back(msg.str());
      }
   }

   void addTriangle(const Triangle& tri)
   {
      LeafGraphicalEntity::addTriangle(tri);
   }
};

///////////////////////////////////////////////////////////////////////////////
