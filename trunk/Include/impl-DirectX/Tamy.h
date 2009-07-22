#pragma once

#include <string>
#include "impl-DirectX\GraphicalCapsEvaluator.h"
#include <d3d9.h>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class D3DInitializer;
class D3DRenderer;
class Renderer;
class SoundRenderer;
class SoundDevice;
class GraphicalEntitiesFactory;
class SoundEntitiesFactory;
class GraphicalDataSource;
class CompositeGraphicalDataSource;
class D3DSceneRenderingMechanism;
class SceneRenderingMechanism;
class SettableRenderingTargetsPolicy;

///////////////////////////////////////////////////////////////////////////////

class Tamy : public GraphicalCapsEvaluator
{
private:
   std::string m_texturesDir;
   std::string m_fontsDir;
   std::string m_meshesDir;

   IDirect3D9* m_d3d9;
   D3DInitializer* m_d3dInitializer;
   SoundRenderer* m_soundRenderer;
   SoundDevice* m_soundDevice;
   D3DRenderer* m_renderer;

   D3DSceneRenderingMechanism* m_sceneRenderingMech;
   SettableRenderingTargetsPolicy* m_sceneRenderingTargetsPolicy;

   GraphicalEntitiesFactory* m_graphicalFactory;
   SoundEntitiesFactory* m_soundFactory;

   CompositeGraphicalDataSource* m_meshLoaders;

public:
   Tamy(const std::string& texturesDir,
        const std::string& fontsDir,
        const std::string& meshesDir);
   ~Tamy();

   void initialize(HWND mainAppWindow);

   bool checkDeviceCaps(const D3DCAPS9& caps);

   Renderer& renderer();
   SoundRenderer& soundRenderer();
   SoundDevice& soundDevice();

   SceneRenderingMechanism& sceneRenderingMechanism();
   SettableRenderingTargetsPolicy& sceneRenderingTargetPolicy();

   GraphicalEntitiesFactory& graphicalFactory();
   SoundEntitiesFactory& soundFactory();

   GraphicalDataSource& meshLoaders();

private:
   D3DRenderer* createRenderer(HWND hWnd, bool windowed);
};

///////////////////////////////////////////////////////////////////////////////
