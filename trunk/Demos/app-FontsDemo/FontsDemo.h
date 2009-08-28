#pragma once

#include "ext-Demo\DemoApp.h"
#include "ext-Fonts\Font.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////


namespace demo
{
   class DemoRendererDefinition;
};

///////////////////////////////////////////////////////////////////////////////

class FontsDemo : public demo::DemoApp
{
private:
   ResourceStorage<Font> m_fontsStorage;

public:
   FontsDemo(Tamy& tamy);

protected:
   void initializeScene(demo::DynMeshesScene& dynamicScene, demo::LightsScene& lights);
   void onDeinitialize();
};

///////////////////////////////////////////////////////////////////////////////
