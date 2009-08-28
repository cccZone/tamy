#include "FontsDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-ResourceManagement\XMLFont.h"
#include "ext-Fonts\VisibleString.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

FontsDemo::FontsDemo(Tamy& tamy)
: DemoApp(tamy)
{
}

///////////////////////////////////////////////////////////////////////////////

void FontsDemo::initializeScene(demo::DynMeshesScene& dynamicScene, 
                                demo::LightsScene& lights)
{   
   XMLFont* font = new XMLFont("..\\Data\\Curlz.fnt", m_tamy.graphicalFactory());
   m_fontsStorage.add(font);


   Light* light3D = m_tamy.graphicalFactory().createLight("light3D");
   lights.insert(light3D);
   light3D->setType(Light::LT_DIRECTIONAL);
   light3D->setDiffuseColor(Color(1, 1, 1, 1));

   VisibleString* string3D = new VisibleString(m_fontsStorage.get("Curlz"));
   dynamicScene.addNode(string3D);
   D3DXMatrixTranslation(&(string3D->accessLocalMtx()), 0, 0, 80);
   D3DXMATRIX helperMtx;
   D3DXMatrixScaling(&helperMtx, 30, 30, 30);
   D3DXMatrixMultiply(&(string3D->accessLocalMtx()), &helperMtx, &(string3D->accessLocalMtx()));
   string3D->setText("Hello World in 3D");
   string3D->setColor(Color(1, 0.5f, 0.5f, 1));
}

///////////////////////////////////////////////////////////////////////////////

void FontsDemo::onDeinitialize()
{   
}

///////////////////////////////////////////////////////////////////////////////

DEMO(FontsDemo)

///////////////////////////////////////////////////////////////////////////////
