#pragma once

#include "GraphicalEntity.h"
#include "Material.h"
#include "TextureStub.h"
#include <list>
#include <string>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntityMock : public GraphicalEntity
{
private:
   static TextureStub s_texture;
   static Material s_material;
   D3DXMATRIX m_mtx;
   std::list<std::string>* m_messageSink;

public:
   GraphicalEntityMock(const std::vector<Material*>& materials) 
         : GraphicalEntity("", materials, D3DXMATRIX()), m_messageSink(NULL)
   {
      D3DXMatrixIdentity(&m_mtx);
   }

   GraphicalEntityMock(const std::string& id, const std::vector<Material*>& materials, std::list<std::string>& messageSink) 
         : GraphicalEntity(id, materials, D3DXMATRIX()),
         m_messageSink(&messageSink)
   {
      D3DXMatrixIdentity(&m_mtx);
   }

   GraphicalEntityMock(const std::string& id, const std::vector<Material*>& materials, const D3DXMATRIX& mtx) 
         : GraphicalEntity(id, materials, mtx),
         m_messageSink(NULL)
   {
      D3DXMatrixIdentity(&m_mtx);
   }

   void render(const D3DXMATRIX& globalMtx, DWORD subset)
   {
      m_mtx = globalMtx;

      if (m_messageSink != NULL)
      {
         std::stringstream msg;
         msg << "Render " << getName() << " subset " << subset;
         m_messageSink->push_back(msg.str());
      }
   }

   const D3DXMATRIX& getMatrixSet() const {return m_mtx;}
};

///////////////////////////////////////////////////////////////////////////////
