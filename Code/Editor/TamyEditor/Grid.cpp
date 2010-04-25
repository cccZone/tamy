#include "Grid.h"
#include "core-Renderer.h"
#include "core\Node.h"
#include "core\ResourcesManager.h"

///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class GridRenderingEffect : public ShaderEffect
   {
      DECLARE_CLASS(GridRenderingEffect)

   private:
      Camera* m_camera;
      Node* m_renderedNode;

   public:
      GridRenderingEffect()
      : m_camera(NULL)
      , m_renderedNode(NULL)
      {
      } 

   protected:
      void onBeginRendering()
      {
         if ((m_camera == NULL) || (m_renderedNode == NULL))
         {
            return;
         }

         D3DXMATRIX worldViewProjMtx = m_renderedNode->getGlobalMtx() 
            * m_camera->getViewMtx() 
            * m_camera->getProjectionMtx();
         shader().setMtx("g_mWorldViewProj", worldViewProjMtx);

         shader().setTechnique("tec0");
      }

      void onAttached(Entity& parent) 
      {
         m_renderedNode = dynamic_cast<Node*> (&parent);
      }

      void onDetached(Entity& parent) 
      {
         m_renderedNode = NULL;
      }

      void onAttached(Model& hostModel) 
      {
         CameraComponent* camComp = hostModel.getComponent<CameraComponent> ();
         if (camComp != NULL)
         {
            m_camera = &(camComp->getCamera());
         }
      }

      void onDetached(Model& hostModel) 
      {
         m_camera = NULL;
      }
   };
   BEGIN_OBJECT(GridRenderingEffect, ShaderEffect)
   END_OBJECT()
 
} // anonymous

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT(Grid, Entity)
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Grid::Grid()
: m_gridLines(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

Grid::Grid(Renderer& renderer, ResourcesManager& rm)
{
   m_gridLines = new LineSegments();
   rm.addResource(m_gridLines, "Editor/Geometry/grid.tgl");

   EffectResource& effectRes = rm.create<EffectResource>("Editor/Shaders/GridRenderingEffect.fx");
   Effect* effect = effectRes.load();
   Renderable* renderable = new Renderable();
   renderable->add(m_gridLines->load());
   renderable->add(effect); 
   this->add(renderable);

   float dim = 100.0f;
   float varPos;
   for (int i = -25; i <= 25; ++i)
   {
      varPos = i * 4.0f;
      m_gridLines->add(LineSegment(D3DXVECTOR3(-dim, 0, varPos), D3DXVECTOR3(dim, 0, varPos)));
      m_gridLines->add(LineSegment(D3DXVECTOR3(varPos, 0, -dim), D3DXVECTOR3(varPos, 0, dim)));
   }
   m_gridLines->rebuild();
}

///////////////////////////////////////////////////////////////////////////////

Grid::~Grid()
{
   m_gridLines = NULL;
}

///////////////////////////////////////////////////////////////////////////////
