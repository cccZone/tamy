#include "core-Renderer\BackgroundPass.h"
#include "core-Renderer\SceneManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\Camera.h"


///////////////////////////////////////////////////////////////////////////////

void BackgroundPass::operator()(SceneManager& sceneManager, Renderer& renderer)
{
   if (sceneManager.isSkyBox() == false) {return;}

   SkyBox& skyBox = sceneManager.getSkyBox();
   skyBox.render();
}

///////////////////////////////////////////////////////////////////////////////
