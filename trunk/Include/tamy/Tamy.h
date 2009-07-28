#pragma once

#include "core\WindowMessagesProcessor.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class GraphicalEntitiesFactory;
class UserInputController;
class CompositeGraphicalDataSource;
class SceneRenderingMechanism;
class ApplicationManager;
class SoundDevice;
class SoundRenderer;
class SoundEntitiesFactory;
class GraphicalDataSource;
class RenderingTargetsPolicy;
class UserInputController;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is the engine factory class.
 * It will build proper implementaiton dependent versions
 * of the basic engine components
 */
class Tamy
{
private:
   static Tamy s_theInstance;

   HWND m_hWnd;
   CompositeWindowMessagesProcessor* m_winMsgsProcessor;
   bool m_fullScreen;

   CompositeGraphicalDataSource* m_meshLoaders;

   SoundRenderer* m_soundRenderer;

public:
   ~Tamy();

   static void initialize(HINSTANCE hInstance,
                          int nCmdShow,
                          const std::string& appName,
                          unsigned int winWidth,
                          unsigned int winHeight,
                          bool fullScreen);

   static Tamy& getInstance() {return s_theInstance;}

   Renderer& renderer();

   GraphicalEntitiesFactory& graphicalFactory();

   /**
    * This method will create a mechanism that allows to render the contents
    * of a single (and multiple) VisualSceneManager(s)
    */
   SceneRenderingMechanism* createSceneRenderingMechanism(RenderingTargetsPolicy* policy);

   /**
    * The method returns the data source which can be used to load
    * supported types of mesh files
    */
   GraphicalDataSource& meshLoaders();

   ApplicationManager& appManager();

   UserInputController& uiController();

   /**
    * This method will create an instance of the UserInputController clas
    */
   UserInputController* createUserInputController();

   SoundDevice& soundDevice();

   SoundRenderer& soundRenderer();

   SoundEntitiesFactory& soundFactory();

private:
   Tamy();
   Tamy(const Tamy& rhs) {}

   void createMainWindow(HINSTANCE hInstance,
                         int nCmdShow,
                         const std::string& appName,
                         unsigned int winWidth,
                         unsigned int winHeight,
                         bool fullScreen);

   void createRenderer(HWND window, 
                       unsigned int winWidth,
                       unsigned int winHeight,
                       bool fullScreen);
   void destroyRenderer();

   void registerMeshLoaders(CompositeGraphicalDataSource& composite);

   void createAppManager(const std::string& appName);
   void destroyAppManager();

   void createSoundSystem();
   void destroySoundSystem();
};

///////////////////////////////////////////////////////////////////////////////

#define TAMY Tamy::getInstance()

///////////////////////////////////////////////////////////////////////////////
