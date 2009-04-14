#include "core-TestFramework\TestFramework.h"
#include "SceneManagerMock.h"
#include "ext-UI\UIManager.h"
#include "ext-UI\Button.h"
#include "core\Node.h"


//////////////////////////////////////////////////////////////////////////////

TEST(UI, clickingButton)
{
   UIManager uiManager;
   Button* button = new Button();

   uiManager.addWidget(button);

   uiManager.action(0, 0);
   CPPUNIT_ASSERT_EQUAL(false, button->wasClicked());

   uiManager.action(100, 50);
   CPPUNIT_ASSERT_EQUAL(true, button->wasClicked());
}

//////////////////////////////////////////////////////////////////////////////
