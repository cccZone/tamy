#include "core-TestFramework\TestFramework.h"
#include "RendererImplementationMock.h"
#include "SkyBoxMock.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\BackgroundPass.h"


///////////////////////////////////////////////////////////////////////////////

TEST(BackgroundPass, renderingSkyBox)
{
   VisualSceneManager sceneManager;
   RendererImplementationMock renderer;

   SkyBoxMock* skyBox = new SkyBoxMock();
   sceneManager.addNode(skyBox);

   BackgroundPass pass;
   pass(sceneManager, renderer); 

   CPPUNIT_ASSERT(skyBox->wasRendered());
}

///////////////////////////////////////////////////////////////////////////////
