#include "core-TestFramework\TestFramework.h"
#include "RendererImplementationMock.h"
#include "SkyBoxMock.h"
#include "VisualSceneManagerMock.h"
#include "core-Renderer\BackgroundPass.h"


///////////////////////////////////////////////////////////////////////////////

TEST(BackgroundPass, renderingSkyBox)
{
   VisualSceneManagerMock sceneManager;
   RendererImplementationMock renderer;

   SkyBoxMock* skyBox = new SkyBoxMock();
   sceneManager.addNode(skyBox);

   BackgroundPass pass;
   pass(sceneManager, renderer); 

   CPPUNIT_ASSERT(skyBox->wasRendered());
}

///////////////////////////////////////////////////////////////////////////////
