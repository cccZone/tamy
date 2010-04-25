#include "ext-Demo\FpsCounter.h"
#include "core-Renderer\TextField.h"
#include "core\Timer.h"


namespace demo
{

///////////////////////////////////////////////////////////////////////////////

FpsCounter::FpsCounter(TextField& textField)
: m_timer(new CTimer()), m_textField(textField)
{}

///////////////////////////////////////////////////////////////////////////////

FpsCounter::~FpsCounter()
{
   delete m_timer;
   m_timer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void FpsCounter::update(float timeElapsed)
{
   m_timer->tick();

   char str[32];
   sprintf_s(str, 32, "FPS: %u", m_timer->getFrameRate());
   m_textField.setText(str);
}

///////////////////////////////////////////////////////////////////////////////

} // namespace demo
