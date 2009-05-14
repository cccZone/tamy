#pragma once

#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "LightReflectingPropertiesStub.h"
#include <list>
#include <string>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntityMock : public GraphicalEntity
{
private:
   static LightReflectingPropertiesStub s_lrp;
   static Material s_material;
   D3DXMATRIX m_mtx;
   std::list<std::string>* m_messageSink;
   std::vector<D3DXMATRIX> m_matrices;

public:
   GraphicalEntityMock(const std::string& id, const std::vector<Material*>& materials, std::list<std::string>& messageSink) 
         : GraphicalEntity(id, materials),
         m_messageSink(&messageSink)
   {
      D3DXMatrixIdentity(&m_mtx);
   }

   GraphicalEntityMock(const std::string& id, const std::vector<Material*>& materials) 
         : GraphicalEntity(id, materials),
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

   void render(const std::vector<D3DXMATRIX>& matrices, DWORD subset)
   {
      m_matrices = matrices;
   }

   const std::vector<D3DXMATRIX>& getMatricesUsedForRendering() const {return m_matrices;}

   const D3DXMATRIX& getMatrixSet() const {return m_mtx;}
};

///////////////////////////////////////////////////////////////////////////////
