#include "core-TestFramework\TestFramework.h"
#include "RendererImplementationMock.h"
#include "SkyBoxMock.h"
#include "SceneManagerMock.h"
#include "core-Renderer\BackgroundPass.h"


///////////////////////////////////////////////////////////////////////////////

TEST(BackgroundPass, renderingSkyBox)
{
   SceneManagerMock sceneManager;
   RendererImplementationMock renderer;

   SkyBoxMock* skyBox = new SkyBoxMock();
   sceneManager.setSkyBox(skyBox);

   BackgroundPass pass;
   pass(sceneManager, renderer); 

   CPPUNIT_ASSERT(skyBox->wasRendered());
}

///////////////////////////////////////////////////////////////////////////////
