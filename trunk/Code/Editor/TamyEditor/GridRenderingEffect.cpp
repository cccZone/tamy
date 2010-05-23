#include "GridRenderingEffect.h"
#include "core-Renderer.h"
#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT(GridRenderingEffect, ShaderEffect)
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

GridRenderingEffect::GridRenderingEffect()
: m_camera(NULL)
, m_renderedNode(NULL)
{
} 

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onBeginRendering()
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

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onAttached(Entity& parent) 
{
   m_renderedNode = dynamic_cast<Node*> (&parent);
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onDetached(Entity& parent) 
{
   m_renderedNode = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onAttached(Model& hostModel) 
{
   m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onDetached(Model& hostModel) 
{
   m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onComponentAdded( Component< Model >& component ) 
{
   CameraComponent* comp = dynamic_cast< CameraComponent* >( &component );
   if ( comp )
   {
      m_camera = &comp->getCamera();
   }
}

///////////////////////////////////////////////////////////////////////////////
