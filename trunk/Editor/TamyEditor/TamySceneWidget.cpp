#include "TamySceneWidget.h"
#include "core\WindowMessagesProcessor.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class NullWindowMessagesProcessor : public IWindowMessagesProcessor
   {
   public:
      bool wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
      {
         return false;
      }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TamySceneWidget::TamySceneWidget(QWidget* parent, 
                                 Qt::WindowFlags f)
: QWidget(parent, f)
, m_winMsgProcessor(new NullWindowMessagesProcessor())
, m_activeWinMsgProcessor(m_winMsgProcessor)
{

   m_hWnd = static_cast<HWND> (winId());
}

///////////////////////////////////////////////////////////////////////////////

TamySceneWidget::~TamySceneWidget()
{
   delete m_winMsgProcessor; m_winMsgProcessor = NULL;
   m_activeWinMsgProcessor = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::setWinMsgProcessor(IWindowMessagesProcessor& winMsgProcessor)
{
   m_activeWinMsgProcessor = &winMsgProcessor;
}

///////////////////////////////////////////////////////////////////////////////

HWND TamySceneWidget::getWinHandle() const 
{
   return m_hWnd;
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::moveEvent(QMoveEvent* event)
{
   UINT message = WM_MOVE;
   WPARAM wParam = 0;
   LPARAM lParam = 0;

   m_activeWinMsgProcessor->wndProc(m_hWnd, message, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////

void TamySceneWidget::resizeEvent(QResizeEvent* event)
{
   UINT message = WM_SIZE;
   WPARAM wParam = 0;
   LPARAM lParam = 0;

   m_activeWinMsgProcessor->wndProc(m_hWnd, message, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
