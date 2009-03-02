#include "ComplexSceneDemo.h"
#include <tchar.h>
#include "WindowBuilder.h"
#include "D3DRenderer.h"
#include "D3DInitializer.h"
#include "UnconstrainedMotionController.h"
#include "Timer.h"
#include <cassert>
#include "BasicSceneManager.h"
#include "GeometryBuilder.h"
#include "GraphicalEntityInstantiator.h"
#include "Camera.h"
#include "Light.h"
#include "D3DResourceManager.h"
#include "IWFLoader.h"
#include "GraphicalEntity.h"
#include "Skeleton.h"
#include "GraphicalEntityLoader.h"


///////////////////////////////////////////////////////////////////////////////

#define IS_PRESSED(keyboardBuffer, keyCode) (keyBuffer[keyCode] & 0xF0)

///////////////////////////////////////////////////////////////////////////////

ComplexSceneDemo::ComplexSceneDemo(HINSTANCE hInstance)
      : m_hInstance(hInstance),
      m_timer(new CTimer()),
      m_lastFrameRate(0),
      m_sceneManager(new BasicSceneManager()),
      m_cameraController(NULL),
      m_animationController(NULL)
{
   CWindowBuilder winBuilder;

   WindowParams params;
   strcpy_s(params.windowTitle, "Complex Scene Demo");
   strcpy_s(params.windowClassName, "ComplexSceneDemoClass");
   params.ptrMsgProc = this;

   m_hWnd = winBuilder.createWindowedModeWindow(m_hInstance, params);
   assert(m_hWnd != NULL);

   m_resourceManager = new D3DResourceManager("..\\Data", "..\\Data", m_hWnd, true);
   m_renderer = &(m_resourceManager->getRendererInstance());
}

///////////////////////////////////////////////////////////////////////////////

ComplexSceneDemo::~ComplexSceneDemo()
{
   delete m_animationController;
   m_animationController = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   m_renderer = NULL;

   delete m_resourceManager;
   m_resourceManager = NULL;

   delete m_timer;
   m_timer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ComplexSceneDemo::run(int nCmdShow)
{
   MSG msg;
   ShowWindow(m_hWnd, nCmdShow);
/*
   IWFLoader loader(*m_resourceManager, *m_sceneManager);
   loader.load("..\\Data\\AnimLandscape.iwf");

   AbstractGraphicalEntity& ent = m_resourceManager->getGraphicalEntity("animlandscape.x");
   m_animationController = ent.instantiateSkeleton(m_sceneManager->getRootNode());
   m_animationController->activateAnimation("Cutscene_01");
*/

   GraphicalEntityLoader& loader =  m_resourceManager->getLoaderForFile("US Ranger.x");
   AbstractGraphicalEntity& ent = m_resourceManager->loadGraphicalEntity("US Ranger.x", loader);
   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("ranger01");
   entInstance->attachEntity(ent);
   m_sceneManager->addNode(entInstance);

   m_animationController = ent.instantiateSkeleton(*entInstance);
   m_animationController->activateAnimation("");

   Light* light = m_resourceManager->createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 0));
   light->setLookVec(D3DXVECTOR3(0, 0, -1));
   m_sceneManager->addNode(light);

   Camera* camera = m_resourceManager->createCamera("camera");
   camera->setLookVec(D3DXVECTOR3(0, -1, -1));
   camera->setPosition(D3DXVECTOR3(0, 40, 30));
   m_sceneManager->addNode(camera);
   m_renderer->setActiveCamera(*camera);
   m_cameraController = new UnconstrainedMotionController(*camera);


   while (1) 
   {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
      {
         if (msg.message == WM_QUIT) break;
         TranslateMessage(&msg);
         DispatchMessage (&msg);
      } 
      else
      {
         advanceGameState();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ComplexSceneDemo::advanceGameState()
{
   m_timer->tick();
   float timeElapsed = m_timer->getTimeElapsed();

   static TCHAR titleBuffer[255];  
   if (m_lastFrameRate != m_timer->getFrameRate())
   {
      m_lastFrameRate = m_timer->getFrameRate();
      _stprintf_s(titleBuffer, _T("Complex Scene Demo : %ld FPS"), m_lastFrameRate);
      SetWindowText(m_hWnd, titleBuffer);
   }

   processInput(timeElapsed);
   m_animationController->update(timeElapsed);
   m_renderer->render(m_sceneManager->getRootNode());
}

///////////////////////////////////////////////////////////////////////////////

void ComplexSceneDemo::processInput(float timeElapsed)
{
   UCHAR keyBuffer[256];
   if (!GetKeyboardState(keyBuffer)) return;

   float movementSpeed = 40;
   float rotationSpeed = 180;


   // process the keys
   if (IS_PRESSED(keyBuffer, VK_ESCAPE))     {PostQuitMessage(0);}
   if (IS_PRESSED(keyBuffer, VK_UP))     {m_cameraController->move(m_cameraController->getLookVec()   * movementSpeed * timeElapsed);}
   if (IS_PRESSED(keyBuffer, VK_DOWN))   {m_cameraController->move(-m_cameraController->getLookVec()  * movementSpeed * timeElapsed);}
   if (IS_PRESSED(keyBuffer, VK_LEFT))   {m_cameraController->move(-m_cameraController->getRightVec() * movementSpeed * timeElapsed);}
   if (IS_PRESSED(keyBuffer, VK_RIGHT))  {m_cameraController->move(m_cameraController->getRightVec()  * movementSpeed * timeElapsed);}

   // process the mouse
   if (GetCapture() == m_hWnd)
   {
      POINT cursorPos;
      GetCursorPos(&cursorPos);
      float rotX = (float)(cursorPos.x - m_oldCursorPos.x) / 3.0f;
      float rotY = (float)(cursorPos.y - m_oldCursorPos.y) / 3.0f;
      SetCursorPos(m_oldCursorPos.x, m_oldCursorPos.y);

      m_cameraController->rotate(rotY * rotationSpeed * timeElapsed, rotX * rotationSpeed * timeElapsed, 0);
   }
}

///////////////////////////////////////////////////////////////////////////////

LRESULT ComplexSceneDemo::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
      case WM_SIZE:
      {
         UINT screenWidth  = LOWORD( lParam );
         UINT screenHeight = HIWORD( lParam );

         if (m_renderer)
         {
            m_renderer->resizeViewport(screenWidth, screenHeight);
         }

         break;
      }

      case WM_LBUTTONDOWN:
      {
         SetCapture(m_hWnd);
         SetCursor(NULL);
         GetCursorPos(&m_oldCursorPos);
         break;
      }

      case WM_LBUTTONUP:
      {
        ReleaseCapture( );
        SetCursor(NULL);
        break;
      }

      case WM_DESTROY:
      {
         PostQuitMessage(0);
         break;
      }

      default:
      {
			return DefWindowProc(hWnd, message, wParam, lParam);
      }
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
	ComplexSceneDemo app(hInstance);
   app.run(nCmdShow);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
