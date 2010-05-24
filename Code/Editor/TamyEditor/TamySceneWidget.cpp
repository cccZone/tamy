#include "TamySceneWidget.h"
#include "core.h"
#include "core-AppFlow.h"
#include "core-MVC.h"
#include "core-Renderer.h"
#include "dx9-Renderer.h"
#include <stdexcept>
#include <QEvent.h>
#include "tamyeditor.h"
#include "DebugRenderer.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   class RendererUpdater : public TimeDependent
   {
   private:
      Renderer& m_renderer;

   public:
      RendererUpdater(Renderer& renderer)
      : m_renderer(renderer)
      {}

      void update(float timeElapsed)
      {
         m_renderer.render();
      }
   };

} // anonymous

IDirect3D9* TamySceneWidget::s_d3d9 = NULL;

///////////////////////////////////////////////////////////////////////////////

TamySceneWidget::TamySceneWidget( QWidget* parent, Qt::WindowFlags f )
: QWidget( parent, f )
, m_keysStatusManager( NULL )
, m_rendererComponent( NULL )
, m_renderer( NULL )
, m_camera( NULL )
, m_scene( NULL )
, m_debugRenderer( NULL )
{
   m_hWnd = static_cast<HWND> (winId());
   memset(m_keyBuffer, 0, sizeof(unsigned char) * 256);

   setFocusPolicy(Qt::ClickFocus);

   m_keysStatusManager = new KeysStatusManager(*this);

   if (s_d3d9 == NULL)
   {
      s_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
      if (s_d3d9 == NULL)
      {
         throw std::runtime_error("Cannot initialize DirectX library");
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

TamySceneWidget::~TamySceneWidget()
{
   m_scene = NULL;

   m_renderer->setMechanism( NULL );
   delete m_debugRenderer; m_debugRenderer = NULL;

   delete m_keysStatusManager; m_keysStatusManager = NULL;

   delete m_camera; m_camera = NULL;

   m_resMgr->removeComponent( *m_rendererComponent );
   delete m_rendererComponent; m_rendererComponent = NULL;
   delete m_renderer; m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::initialize( TamyEditor& mgr )
{
   mgr.addToMainWidget(this);

   m_resMgr = &mgr.requestService< ResourcesManager >();

   // create a renderer
   SimpleTamyConfigurator configurator;
   RenderingDevice* device = configurator.selectRenderingDevice(*s_d3d9, 800, 600, false);

   DX9Initializer initializer(*s_d3d9);
   m_renderer = initializer.createDisplay(DX9Settings(*device), m_hWnd);
   delete device;

   // create a camera
   m_camera = new Camera("Camera", *m_renderer);
   D3DXMatrixTranslation(&(m_camera->accessLocalMtx()), 0, 5, 0);


   // create a debug renderer
   m_debugRenderer = new DebugRenderer( *m_renderer, *m_camera );

   // register new services
   mgr.registerService< Renderer >( *this, *m_renderer );
   mgr.registerService< Camera >( *this, *m_camera );
   mgr.registerService< KeysStatusManager >( *this, *m_keysStatusManager );
   mgr.registerService< UserInputController >( *this, *this );
   mgr.registerService< DebugRenderer >( *this, *m_debugRenderer );

   m_rendererComponent = new RendererComponent( *m_renderer );
   m_resMgr->addComponent( m_rendererComponent );

   createRenderer( mgr );
   setupTimeController( mgr );
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::onServiceRegistered( TamyEditor& mgr )
{
   if ( mgr.needsUpdate< Model >( *m_scene ) )
   {
      createRenderer( mgr );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::setupTimeController( TamyEditor& mgr )
{
   TimeController& timeController = mgr.requestService<TimeController> ();
   timeController.remove("rendering");
   timeController.add("rendering");
   timeController.get("rendering").add(new RendererUpdater(*m_renderer));
   timeController.remove("input");
   timeController.add("input");
   timeController.get("input").add(new TTimeDependent<UserInputController> (*this));
   timeController.get("input").add(new TTimeDependent<KeysStatusManager> (*m_keysStatusManager));
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::createRenderer( TamyEditor& mgr )
{
   // create a rendering mechanism
   CompositeRenderingMechanism* compRM = new CompositeRenderingMechanism();

   // render the main scene first ( if there's one )
   if ( mgr.hasService< Model >() )
   {
      m_scene = &mgr.requestService< Model >();
      m_scene->addComponent( new CameraComponent( *m_camera ) );

      Scene3DRM* sceneRenderingMech = new Scene3DRM( *m_scene, *m_camera );
      compRM->add( sceneRenderingMech );
   }
   else
   {
      m_scene = NULL;
   }

   // renderer the debug info
   Scene3DRM* debugRenderingMech = new Scene3DRM( m_debugRenderer->getModel(), *m_camera );
   compRM->add( debugRenderingMech );

   m_renderer->setMechanism( compRM );
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::moveEvent(QMoveEvent* event)
{
   WINDOWINFO info;
   GetWindowInfo(m_hWnd, &info);

   m_renderer->resizeViewport(info.rcClient.right - info.rcClient.left, 
      info.rcClient.bottom - info.rcClient.top, 
      info.rcClient.left, 
      info.rcClient.top,
      info.rcClient.right, 
      info.rcClient.bottom);
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::resizeEvent(QResizeEvent* event)
{
   WINDOWINFO info;
   GetWindowInfo(m_hWnd, &info);

   m_renderer->resizeViewport(info.rcClient.right - info.rcClient.left, 
      info.rcClient.bottom - info.rcClient.top, 
      info.rcClient.left, 
      info.rcClient.top,
      info.rcClient.right, 
      info.rcClient.bottom);
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::keyPressEvent(QKeyEvent* event)
{
   setKey(m_keyBuffer, toDXKey(event->key()), true);
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::keyReleaseEvent(QKeyEvent* event)
{
   setKey(m_keyBuffer, toDXKey(event->key()), false);
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::mousePressEvent(QMouseEvent* event)
{
   switch(event->button())
   {
   case Qt::LeftButton:
      {
         setKey(m_keyBuffer, VK_LBUTTON, true);
         break;
      }

   case Qt::RightButton:
      {
         setKey(m_keyBuffer, VK_RBUTTON, true);
         break;
      }

   case Qt::MidButton:  // fall through
   case Qt::NoButton:   // fall through
   default:
      {
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::mouseReleaseEvent(QMouseEvent* event)
{
   switch(event->button())
   {
   case Qt::LeftButton:
      {
         setKey(m_keyBuffer, VK_LBUTTON, false);
         break;
      }

   case Qt::RightButton:
      {
         setKey(m_keyBuffer, VK_RBUTTON, false);
         break;
      }

   case Qt::MidButton:  // fall through
   case Qt::NoButton:   // fall through
   default:
      {
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::onRelativeMouseMovement()
{
   this->cursor().setShape(Qt::BlankCursor);
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::onAbsoluteMouseMovement()
{
   this->cursor().setShape(Qt::ArrowCursor);
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::checkUserInput(unsigned char* keyBuffer, Point& mousePos)
{
   memcpy_s(keyBuffer, sizeof(unsigned char) * 256, 
            m_keyBuffer, sizeof(unsigned char) * 256);

   QPoint cursorPos = this->cursor().pos();
   mousePos.x = cursorPos.x();
   mousePos.y = cursorPos.y();
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::setMousePos(const Point& pos)
{
   this->cursor().setPos(QPoint(pos.x, pos.y));
}

///////////////////////////////////////////////////////////////////////////////

unsigned char TamySceneWidget::toDXKey(int qtKeyCode) const
{
   switch (qtKeyCode)
   {
   case Qt::Key_Escape:    return VK_ESCAPE;
   case Qt::Key_Tab:       return VK_TAB;
   case Qt::Key_Backspace: return VK_CLEAR;
   case Qt::Key_Return:    return VK_RETURN;
   case Qt::Key_Enter:     return VK_TAB;
   case Qt::Key_Insert:    return VK_INSERT;
   case Qt::Key_Delete:    return VK_DELETE;
   case Qt::Key_Pause:     return VK_PAUSE;
   case Qt::Key_Print:     return VK_PRINT;
   case Qt::Key_Home:      return VK_HOME;
   case Qt::Key_End:       return VK_END;
   case Qt::Key_Left:      return VK_LEFT;
   case Qt::Key_Up:        return VK_UP;
   case Qt::Key_Right:     return VK_RIGHT;
   case Qt::Key_Down:      return VK_DOWN;
   case Qt::Key_PageUp:    return VK_PRIOR;
   case Qt::Key_PageDown:  return VK_NEXT;
   case Qt::Key_Shift:     return VK_SHIFT;
   case Qt::Key_Control:   return VK_CONTROL;
   case Qt::Key_Clear:     return VK_CLEAR;
   case Qt::Key_F1:        return VK_F1;
   case Qt::Key_F2:        return VK_F2;
   case Qt::Key_F3:        return VK_F3;
   case Qt::Key_F4:        return VK_F4;
   case Qt::Key_F5:        return VK_F5;
   case Qt::Key_F6:        return VK_F6;
   case Qt::Key_F7:        return VK_F7;
   case Qt::Key_F8:        return VK_F8;
   case Qt::Key_F9:        return VK_F9;
   case Qt::Key_F10:       return VK_F10;
   case Qt::Key_F11:       return VK_F11;
   case Qt::Key_F12:       return VK_F12;
   case Qt::Key_F13:       return VK_F13;
   case Qt::Key_F14:       return VK_F14;
   case Qt::Key_F15:       return VK_F15;
   case Qt::Key_F16:       return VK_F16;
   case Qt::Key_F17:       return VK_F17;
   case Qt::Key_F18:       return VK_F18;
   case Qt::Key_F19:       return VK_F19;
   case Qt::Key_F20:       return VK_F20;
   case Qt::Key_F21:       return VK_F21;
   case Qt::Key_F22:       return VK_F22;
   case Qt::Key_F23:       return VK_F23;
   case Qt::Key_F24:       return VK_F24;
   case Qt::Key_Space:     return VK_SPACE;
   case Qt::Key_Exclam:    return '!';
   case Qt::Key_QuoteDbl:  return '"';
   case Qt::Key_NumberSign:return '#';
   case Qt::Key_Dollar:    return '$';
   case Qt::Key_Percent:   return '%';
   case Qt::Key_Ampersand: return '&';
   case Qt::Key_Apostrophe:return '\'';
   case Qt::Key_ParenLeft: return '(';
   case Qt::Key_ParenRight:return ')';   
   case Qt::Key_Asterisk:  return '*';
   case Qt::Key_Plus:      return '+';
   case Qt::Key_Comma:     return ',';
   case Qt::Key_Minus:     return '-';
   case Qt::Key_Period:    return '.';
   case Qt::Key_Slash:     return '/';
   case Qt::Key_0:         return '0';
   case Qt::Key_1:         return '1';
   case Qt::Key_2:         return '2';
   case Qt::Key_3:         return '3';
   case Qt::Key_4:         return '4';
   case Qt::Key_5:         return '5';
   case Qt::Key_6:         return '6';
   case Qt::Key_7:         return '7';
   case Qt::Key_8:         return '8';
   case Qt::Key_9:         return '9';
   case Qt::Key_Colon:     return ':';
   case Qt::Key_Semicolon: return ';';
   case Qt::Key_Less:      return '<';
   case Qt::Key_Equal:     return '=';
   case Qt::Key_Greater:   return '>';
   case Qt::Key_Question:  return '?';
   case Qt::Key_At:        return '@';
   case Qt::Key_A:         return 'A';
   case Qt::Key_B:         return 'B';
   case Qt::Key_C:         return 'C';
   case Qt::Key_D:         return 'D';
   case Qt::Key_E:         return 'E';
   case Qt::Key_F:         return 'F';
   case Qt::Key_G:         return 'G';
   case Qt::Key_H:         return 'H';
   case Qt::Key_I:         return 'I';
   case Qt::Key_J:         return 'J';
   case Qt::Key_K:         return 'K';
   case Qt::Key_L:         return 'L';
   case Qt::Key_M:         return 'M';
   case Qt::Key_N:         return 'N';
   case Qt::Key_O:         return 'O';
   case Qt::Key_P:         return 'P';
   case Qt::Key_Q:         return 'Q';
   case Qt::Key_R:         return 'R';
   case Qt::Key_S:         return 'S';
   case Qt::Key_T:         return 'T';
   case Qt::Key_U:         return 'U';
   case Qt::Key_V:         return 'V';
   case Qt::Key_W:         return 'W';
   case Qt::Key_X:         return 'X';
   case Qt::Key_Y:         return 'Y';
   case Qt::Key_Z:         return 'Z';
   case Qt::Key_BracketLeft:  return '[';
   case Qt::Key_Backslash:    return '\\';
   case Qt::Key_BracketRight: return ']';
   case Qt::Key_Underscore:   return '_';
   case Qt::Key_BraceLeft:    return '{';
   case Qt::Key_Bar:          return '|';
   case Qt::Key_BraceRight:   return '}';
   case Qt::Key_AsciiTilde:   return '~';
   default:                return 0;
   };
}

///////////////////////////////////////////////////////////////////////////////
