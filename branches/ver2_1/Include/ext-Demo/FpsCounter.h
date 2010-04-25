#pragma once

#include "core-AppFlow\TimeDependent.h"


///////////////////////////////////////////////////////////////////////////////

class CTimer;
class TextField;

///////////////////////////////////////////////////////////////////////////////

namespace demo
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This simple controller will count the FPSes and output them
 * using a text field overlay as a view.
 */
class FpsCounter : public TimeDependent
{
private:
   CTimer* m_timer;
   TextField& m_textField;

public:
   FpsCounter(TextField& textField);
   ~FpsCounter();

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo
