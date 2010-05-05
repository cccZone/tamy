#include "SelectionMarker.h"
#include "core\ResourcesManager.h"
#include "core-Renderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SelectionMarker, ShaderEffect )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Effect* SelectionMarker::create( ResourcesManager& rm )
{
   std::string shaderFileName = "Editor/Shaders/SelectionMarker.fx";
   Shader* shader = NULL;
   if ( ( shader = dynamic_cast< Shader* >( rm.findResource( shaderFileName ) ) ) == NULL )
   {
      shader = new Shader( shaderFileName );
      rm.addResource( shader );
   }

   SelectionMarker* effect = new SelectionMarker();
   effect->initialize( *shader );

   return effect;
}

///////////////////////////////////////////////////////////////////////////////

SelectionMarker::SelectionMarker()
: m_camera( NULL )
, m_renderedNode( NULL )
{
   addAttribute( new TransparencyAttribute() );
} 

///////////////////////////////////////////////////////////////////////////////

void SelectionMarker::onBeginRendering()
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

void SelectionMarker::onAttached(Entity& parent) 
{
   m_renderedNode = dynamic_cast<Node*> (&parent);
}

///////////////////////////////////////////////////////////////////////////////

void SelectionMarker::onDetached(Entity& parent) 
{
   m_renderedNode = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SelectionMarker::onAttached(Model& hostModel) 
{
   CameraComponent* camComp = hostModel.getComponent<CameraComponent> ();
   if (camComp != NULL)
   {
      m_camera = &(camComp->getCamera());
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionMarker::onDetached(Model& hostModel) 
{
   m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////
