#pragma once

#include <string>
#include "impl-DirectX\GraphicalCapsEvaluator.h"
#include <d3d9.h>
#include <windows.h>
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "ext-Fonts\Font.h"
#include "core\ResourceStorage.h"


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

   GraphicalEntitiesFactory* m_graphicalFactory;
   SoundEntitiesFactory* m_soundFactory;

   ResourceStorage<AbstractGraphicalEntity>* m_entitiesStorage;
   ResourceStorage<Material>* m_materialsStorage;
   ResourceStorage<Font>* m_fontsStorage;

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
   GraphicalEntitiesFactory& graphicalFactory();
   SoundEntitiesFactory& soundFactory();

   ResourceStorage<AbstractGraphicalEntity>& entitiesStorage();
   ResourceStorage<Material>& materialsStorage();
   ResourceStorage<Font>& fontsStorage();

   GraphicalDataSource& meshLoaders();

private:
   D3DRenderer* createRenderer(HWND hWnd, bool windowed);
};

///////////////////////////////////////////////////////////////////////////////
